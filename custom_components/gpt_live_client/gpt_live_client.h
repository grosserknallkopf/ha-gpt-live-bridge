// ESPHome GPT Live client interface. Runtime implementation is provided for ESP-IDF builds.
#pragma once
#include "esphome/core/component.h"
#include "esphome/components/microphone/microphone.h"
#include "esphome/components/speaker/speaker.h"
#include <string>
namespace esphome { namespace gpt_live_client {
class GPTLiveClient : public Component { public: void setup() override; void loop() override; void dump_config() override; void connect(); void disconnect(); bool is_connected() const { return connected_; } void set_bridge_url(const std::string& v){bridge_url_=v;} void set_microphone(microphone::Microphone* v){mic_=v;} void set_speaker(speaker::Speaker* v){speaker_=v;} void set_aec_reference(microphone::Microphone* v){aec_ref_=v;} void set_chunk_size_ms(uint32_t v){chunk_size_ms_=v;} void set_send_buffer_size(uint32_t v){send_buf_size_=v;} void set_recv_buffer_size(uint32_t v){recv_buf_size_=v;} protected: std::string bridge_url_; microphone::Microphone* mic_{nullptr}; microphone::Microphone* aec_ref_{nullptr}; speaker::Speaker* speaker_{nullptr}; uint32_t chunk_size_ms_{20},send_buf_size_{4096},recv_buf_size_{16384}; bool connected_{false},connecting_{false}; void send_audio(const uint8_t*,size_t); void receive_audio(const uint8_t*,size_t); void handle_message(const char*,size_t);};
}}
