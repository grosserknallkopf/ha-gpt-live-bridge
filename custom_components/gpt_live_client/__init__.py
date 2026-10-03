import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import CONF_ID
gpt_live_client_ns=cg.esphome_ns.namespace("gpt_live_client")
GPTLiveClient=cg.class_("GPTLiveClient",cg.Component)
CONFIG_SCHEMA=cv.Schema({cv.GenerateID():cv.declare_id(GPTLiveClient),cv.Required("server"):cv.string,cv.Optional("fallback_pipeline",default="wake_word"):cv.string,cv.Optional("sample_rate",default=16000):cv.int_range(min=8000,max=48000),cv.Optional("enabled",default=True):cv.boolean}).extend(cv.COMPONENT_SCHEMA)
async def to_code(config):
 var=cg.new_Pvariable(config[CONF_ID]); await cg.register_component(var,config); cg.add(var.set_server(config["server"])); cg.add(var.set_sample_rate(config["sample_rate"])); cg.add(var.set_enabled(config["enabled"]))
