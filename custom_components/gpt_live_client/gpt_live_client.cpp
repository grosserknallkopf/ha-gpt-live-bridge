#include "gpt_live_client.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "esphome/components/network/util.h"

#ifdef USE_ESP_IDF
#include <esp_websocket_client.h>
#include <esp_event.h>
#include <esp_tls.h>
#include <cJSON.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#endif

namespace esphome {
namespace gpt_live_client {

static const char *const TAG = "gpt_live";

// Statischer Event-Handler für esp_websocket_client
static void ws_event_handler(void *handler_args, esp_event_base_t base,
                             int32_t event_id, void *event_data) {
  auto *client = static_cast<GPTLiveClient *>(handler_args);
  auto *data = static_cast<esp_websocket_event_data_t *>(event_data);

  switch (event_id) {
    case WEBSOCKET_EVENT_CONNECTED:
      ESP_LOGI(TAG, "WebSocket connected");
      client->on_ws_connected();
      break;

    case WEBSOCKET_EVENT_DISCONNECTED:
      ESP_LOGI(TAG, "WebSocket disconnected");
      client->on_ws_disconnected();
      break;

    case WEBSOCKET_EVENT_DATA:
      if (data->op_code == 2) {
        // Binary frame = PCM Audio vom Server
        client->receive_audio_chunk(
            std::vector<uint8_t>(data->data_ptr, data->data_ptr + data->data_len));
      } else if (data->op_code == 1) {
        // Text frame = JSON Kommando
        client->handle_server_message(
            std::string(data->data_ptr, data->data_len));
      }
      break;

    case WEBSOCKET_EVENT_ERROR:
      ESP_LOGE(TAG, "WebSocket error");
      break;

    case WEBSOCKET_EVENT_CLOSED:
      ESP_LOGI(TAG, "WebSocket closed");
      break;
  }
}

void GPTLiveClient::setup() {
  ESP_LOGCONFIG(TAG, "GPT-Live Client setup");
  ESP_LOGCONFIG(TAG, "  Bridge URL: %s", bridge_url_.c_str());
  ESP_LOGCONFIG(TAG, "  Chunk size: %d ms", chunk_size_ms_);
  ESP_LOGCONFIG(TAG, "  Send buffer: %d bytes", send_buf_size_);
  ESP_LOGCONFIG(TAG, "  Recv buffer: %d bytes", recv_buf_size_);
}

void GPTLiveClient::dump_config() {
  ESP_LOGCONFIG(TAG, "GPT-Live Client:");
  ESP_LOGCONFIG(TAG, "  Bridge: %s", bridge_url_.c_str());
  ESP_LOGCONFIG(TAG, "  Connected: %s", connected_ ? "YES" : "NO");
}

void GPTLiveClient::loop() {
  // Ausgehende Audio-Chunks senden
  if (!send_queue_.empty() && connected_ && ws_client_ != nullptr) {
    std::lock_guard<std::mutex> lock(send_mutex_);
    while (!send_queue_.empty()) {
      auto &chunk = send_queue_.front();
      esp_websocket_client_send_bin(
          ws_client_,
          reinterpret_cast<const char *>(chunk.data()),
          chunk.size(),
          pdMS_TO_TICKS(50));
      send_queue_.pop();
    }
  }

  // Eingehende Audio-Chunks abspielen
  if (!recv_queue_.empty() && speaker_ != nullptr) {
    std::lock_guard<std::mutex> lock(recv_mutex_);
    while (!recv_queue_.empty()) {
      auto &chunk = recv_queue_.front();
      speaker_->play(chunk.data(), chunk.size());
      recv_queue_.pop();
    }
  }
}

void GPTLiveClient::connect() {
  if (connected_ || connecting_) return;
  connecting_ = true;

  ESP_LOGI(TAG, "Connecting to bridge: %s", bridge_url_.c_str());

  esp_websocket_client_config_t ws_cfg = {};
  ws_cfg.uri = bridge_url_.c_str();
  ws_cfg.buffer_size = recv_buf_size_;
  ws_cfg.task_stack = 8192;
  ws_cfg.task_prio = 5;
  ws_cfg.keep_alive_enable = true;
  ws_cfg.disable_auto_reconnect = true;
  ws_cfg.reconnect_timeout_ms = 5000;

  ws_client_ = esp_websocket_client_init(&ws_cfg);
  esp_websocket_register_events(ws_client_, WEBSOCKET_EVENT_ANY,
                                ws_event_handler, this);
  esp_websocket_client_start(ws_client_);
}

void GPTLiveClient::on_ws_connected() {
  connecting_ = false;
  // Session-Start-Kommando senden
  const char *connect_msg = "{\"type\":\"connect\",\"instructions\":\"Du bist Jarvis.\"}";
  esp_websocket_client_send_text(ws_client_, connect_msg,
                                  strlen(connect_msg), pdMS_TO_TICKS(200));
}

void GPTLiveClient::on_ws_disconnected() {
  connected_ = false;
  connecting_ = false;
  ws_client_ = nullptr;

  // Disconnected Trigger auslösen
  for (auto *trigger : disconnected_triggers_) {
    trigger->trigger();
  }
  ESP_LOGI(TAG, "Disconnected");
}

void GPTLiveClient::disconnect() {
  if (!connected_ && !connecting_) return;

  if (ws_client_ != nullptr) {
    const char *disconnect_msg = "{\"type\":\"disconnect\"}";
    esp_websocket_client_send_text(ws_client_, disconnect_msg,
                                    strlen(disconnect_msg), pdMS_TO_TICKS(200));
    vTaskDelay(pdMS_TO_TICKS(100));
    esp_websocket_client_stop(ws_client_);
    esp_websocket_client_destroy(ws_client_);
    ws_client_ = nullptr;
  }

  connected_ = false;
  connecting_ = false;

  for (auto *trigger : disconnected_triggers_) {
    trigger->trigger();
  }
}

void GPTLiveClient::send_audio_chunk(const std::vector<uint8_t> &data) {
  if (!connected_) return;
  std::lock_guard<std::mutex> lock(send_mutex_);
  send_queue_.push(data);
}

void GPTLiveClient::receive_audio_chunk(const std::vector<uint8_t> &data) {
  std::lock_guard<std::mutex> lock(recv_mutex_);
  recv_queue_.push(data);
}

void GPTLiveClient::handle_server_message(const std::string &msg) {
  cJSON *root = cJSON_Parse(msg.c_str());
  if (!root) return;

  cJSON *type_json = cJSON_GetObjectItem(root, "type");
  if (!type_json || !cJSON_IsString(type_json)) {
    cJSON_Delete(root);
    return;
  }

  std::string type_str = type_json->valuestring;

  if (type_str == "connected") {
    connected_ = true;
    ESP_LOGI(TAG, "GPT-Live session active");
    for (auto *trigger : connected_triggers_) {
      trigger->trigger();
    }
  } else if (type_str == "transcript") {
    cJSON *text = cJSON_GetObjectItem(root, "text");
    cJSON *role = cJSON_GetObjectItem(root, "role");
    if (text && role && cJSON_IsString(text) && cJSON_IsString(role)) {
      for (auto *trigger : transcript_triggers_) {
        trigger->trigger(text->valuestring, role->valuestring);
      }
    }
  } else if (type_str == "error") {
    cJSON *code = cJSON_GetObjectItem(root, "code");
    int err_code = code ? code->valueint : -1;
    ESP_LOGE(TAG, "Server error: %s (code: %d)", 
             cJSON_GetObjectItem(root, "message") ? 
             cJSON_GetObjectItem(root, "message")->valuestring : "unknown",
             err_code);
    for (auto *trigger : error_triggers_) {
      trigger->trigger(err_code);
    }
  } else if (type_str == "disconnected") {
    disconnect();
  } else if (type_str == "function_call") {
    cJSON *name_json = cJSON_GetObjectItem(root, "name");
    if (name_json) {
      ESP_LOGI(TAG, "Function call: %s", name_json->valuestring);
    }
  }

  cJSON_Delete(root);
}

bool GPTLiveClient::is_connected() const {
  return connected_;
}

void GPTLiveClient::add_on_connected_callback(Trigger<> *trigger) {
  connected_triggers_.push_back(trigger);
}
void GPTLiveClient::add_on_disconnected_callback(Trigger<> *trigger) {
  disconnected_triggers_.push_back(trigger);
}
void GPTLiveClient::add_on_transcript_callback(Trigger<std::string, std::string> *trigger) {
  transcript_triggers_.push_back(trigger);
}
void GPTLiveClient::add_on_error_callback(Trigger<int> *trigger) {
  error_triggers_.push_back(trigger);
}

}  // namespace gpt_live_client
}  // namespace esphome
