import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import microphone, speaker
from esphome.const import CONF_ID

DEPENDENCIES = ['wifi']
gpt_live_client_ns = cg.esphome_ns.namespace('gpt_live_client')
GPTLiveClient = gpt_live_client_ns.class_('GPTLiveClient', cg.Component)
CONFIG_SCHEMA = cv.Schema({cv.GenerateID(): cv.declare_id(GPTLiveClient),cv.Required('bridge_url'): cv.string,cv.Required('microphone'): cv.use_id(microphone.Microphone),cv.Required('speaker'): cv.use_id(speaker.Speaker),cv.Optional('aec_reference'): cv.use_id(microphone.Microphone),cv.Optional('chunk_size_ms',default=20): cv.int_range(min=10,max=100),cv.Optional('send_buffer_size',default=4096): cv.int_range(min=512),cv.Optional('recv_buffer_size',default=16384): cv.int_range(min=1024),cv.Optional('on_connected'): automation.validate_automation(single=True),cv.Optional('on_disconnected'): automation.validate_automation(single=True),cv.Optional('on_transcript'): automation.validate_automation(single=True),cv.Optional('on_error'): automation.validate_automation(single=True)}).extend(cv.COMPONENT_SCHEMA)
async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID]); await cg.register_component(var, config)
    cg.add(var.set_bridge_url(config['bridge_url'])); cg.add(var.set_microphone(await cg.get_variable(config['microphone']))); cg.add(var.set_speaker(await cg.get_variable(config['speaker'])))
    cg.add(var.set_chunk_size_ms(config['chunk_size_ms'])); cg.add(var.set_send_buffer_size(config['send_buffer_size'])); cg.add(var.set_recv_buffer_size(config['recv_buffer_size']))
