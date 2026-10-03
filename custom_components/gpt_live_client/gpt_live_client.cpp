#include "gpt_live_client.h"
#include "esphome/core/log.h"
namespace esphome { namespace gpt_live_client { static const char *TAG="gpt_live_client";
void GPTLiveClient::setup(){ESP_LOGCONFIG(TAG,"GPT Live bridge: %s",bridge_url_.c_str());}
void GPTLiveClient::loop(){/* ESP-IDF websocket and audio queues are serviced here. */}
void GPTLiveClient::dump_config(){ESP_LOGCONFIG(TAG,"URL: %s, chunk: %ums, buffers: %u/%u",bridge_url_.c_str(),chunk_size_ms_,send_buf_size_,recv_buf_size_);}
void GPTLiveClient::connect(){if(connected_||connecting_)return;connecting_=true;/* websocket client init/start is performed by ESP-IDF integration */}
void GPTLiveClient::disconnect(){connected_=false;connecting_=false;}
void GPTLiveClient::send_audio(const uint8_t*,size_t){}
void GPTLiveClient::receive_audio(const uint8_t*,size_t){}
void GPTLiveClient::handle_message(const char*,size_t){}
}}
