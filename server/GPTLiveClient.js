const { createOpenAI } = require('@ai-sdk/openai');
const EventEmitter = require('events');

class GPTLiveClient extends EventEmitter {
  constructor(config = {}) { super(); this.config=config; this.ws=null; this.connected=false; this.reconnectTimer=null; this.retries=0; this.pendingCalls=new Map(); }
  async connect() {
    if (this.ws || this.connected) return;
    const apiKey=this.config.apiKey || process.env.VERCEL_API_KEY;
    if (!apiKey) throw new Error('VERCEL_API_KEY is required');
    const openai=createOpenAI({baseURL:'https://ai-gateway.vercel.sh/v1',apiKey});
    const realtime=openai.experimental_realtime(this.config.model || process.env.MODEL || 'openai/gpt-live-1',{api:'live'});
    try { this.ws=await realtime.connect(); } catch(e) { this.emit('error',e); this.scheduleReconnect(); return; }
    this.connected=true; this.retries=0; this.emit('connected');
    this.ws.on('message',(data)=>this.handleMessage(data));
    this.ws.on('close',()=>this.onClose()); this.ws.on('error',(e)=>this.emit('error',e));
    this.send({type:'session.start',session:{voice:this.config.voice||process.env.VOICE||'alloy',instructions:this.config.instructions||process.env.INSTRUCTIONS||'',modalities:['text','audio']}});
  }
  sendAudio(buffer) { if(this.connected) this.send({type:'input_audio_buffer.append',audio:Buffer.from(buffer).toString('base64')}); }
  endAudio() { if(this.connected) this.send({type:'input_audio_buffer.commit'}); }
  send(message) { if(this.ws && this.connected && this.ws.send) this.ws.send(JSON.stringify(message)); }
  handleMessage(raw) { let m; try { m=JSON.parse(Buffer.isBuffer(raw)?raw.toString():raw); } catch { return; }
    const t=m.type||''; if(t==='response.audio.delta'||t==='audio.delta'||t.endsWith('.audio.delta')) this.emit('audio',Buffer.from(m.delta||m.audio||'','base64'));
    if(t.includes('transcript')) this.emit('transcript',{role:m.role||m.response?.role||'assistant',text:m.transcript||m.text||m.delta||''});
    if(t==='response.function_call_arguments.done'||t==='function_call') this.emit('function-call',m);
    if(t==='delegation'||t==='response.created') this.emit('delegation',m);
    if(t==='session.end'||t==='response.done') this.emit('response-done',m);
    if(t==='error') this.emit('error',new Error(m.error?.message||m.message||'GPT Live error'));
  }
  async handleFunctionResult(callId,output) { this.send({type:'conversation.item.create',item:{type:'function_call_output',call_id:callId,output:JSON.stringify(output)}}); this.send({type:'response.create'}); }
  onClose(){this.connected=false;this.ws=null;this.emit('disconnected');if(!this.config.stopping)this.scheduleReconnect();}
  scheduleReconnect(){clearTimeout(this.reconnectTimer);const delay=Math.min(30000,1000*2**this.retries++);this.reconnectTimer=setTimeout(()=>this.connect().catch(e=>this.emit('error',e)),delay);}
  disconnect(){this.config.stopping=true;clearTimeout(this.reconnectTimer);if(this.ws?.close)this.ws.close();this.ws=null;this.connected=false;this.emit('disconnected');}
}
module.exports=GPTLiveClient;
