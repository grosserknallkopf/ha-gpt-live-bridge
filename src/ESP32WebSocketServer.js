import {WebSocketServer,WebSocket} from "ws";
export class ESP32WebSocketServer { constructor({port,onAudio,onOpen,onClose,onControl}){Object.assign(this,{port,onAudio,onOpen,onClose,onControl,clients:new Set()});}
 start(){this.wss=new WebSocketServer({port:this.port});this.wss.on("connection",ws=>{this.clients.add(ws);onOpen?.();ws.on("message",d=>{try{const m=JSON.parse(d);if(m.type==="audio-input"&&m.audio) this.onAudio(Buffer.from(m.audio,"base64")); else this.onControl?.(m)}catch{this.onAudio(Buffer.from(d))}});ws.on("close",()=>{this.clients.delete(ws);this.onClose?.()})})}
 broadcastAudio(b){this.broadcast({type:"audio-output",audio:b.toString("base64")})} broadcast(o){const s=JSON.stringify(o);for(const c of this.clients)if(c.readyState===WebSocket.OPEN)c.send(s)} }
