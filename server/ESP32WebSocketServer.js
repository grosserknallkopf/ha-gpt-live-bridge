const { WebSocketServer, WebSocket } = require('ws');
const GPTLiveClient = require('./GPTLiveClient');
const tools = require('./ha-mcp-tools');
class ESP32WebSocketServer {
 constructor(config={}){this.config=config;this.wss=null;this.clients=new Set();}
 start(){this.wss=new WebSocketServer({port:Number(this.config.port||8090)});this.wss.on('connection',(socket)=>this.accept(socket));console.log(`ESP32 WebSocket server listening on ${this.config.port||8090}`);}
 accept(socket){const live=new GPTLiveClient(this.config);const client={socket,live};this.clients.add(client);
  const send=(x)=>socket.readyState===WebSocket.OPEN&&socket.send(typeof x==='string'?x:JSON.stringify(x));
  live.on('connected',()=>send({type:'connected'}));live.on('disconnected',()=>send({type:'disconnected'}));live.on('audio',(b)=>socket.readyState===WebSocket.OPEN&&socket.send(b));live.on('transcript',(x)=>send({type:'transcript',...x}));live.on('error',(e)=>send({type:'error',message:e.message,code:500}));
  live.on('function-call',async c=>{try{const name=c.name||c.function?.name,args=typeof c.arguments==='string'?JSON.parse(c.arguments||'{}'):c.arguments||{};const fn=tools[name];const out=fn?await fn(args,this.config):{error:`Unknown tool ${name}`};await live.handleFunctionResult(c.call_id||c.id,out);}catch(e){send({type:'error',message:e.message,code:500});}});
  socket.on('message',(data,isBinary)=>{if(isBinary)return live.sendAudio(data);let m;try{m=JSON.parse(data.toString())}catch{return send({type:'error',message:'Invalid JSON',code:400});}if(m.type==='connect') {live.config.instructions=m.instructions||this.config.instructions;live.config.stopping=false;live.connect().catch(e=>send({type:'error',message:e.message,code:500}));}else if(m.type==='disconnect')live.disconnect();else if(m.type==='ping')send({type:'pong'});else if(m.type==='audio.commit')live.endAudio();});
  socket.on('close',()=>{live.disconnect();this.clients.delete(client);});
 }
 stop(){for(const c of this.clients)c.live.disconnect();this.wss?.close();}
}
module.exports=ESP32WebSocketServer;
