#pragma once
#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include <string>
namespace esphome { namespace gpt_live_client {
class GPTLiveClient: public Component { public: void set_server(const std::string& s){server_=s;} void set_sample_rate(int s){sample_rate_=s;} void set_enabled(bool e){enabled_=e;} void setup() override {} void loop() override {} float get_setup_priority() const override {return setup_priority::AFTER_CONNECTION;} private: std::string server_; int sample_rate_{16000}; bool enabled_{true}; };
}}
