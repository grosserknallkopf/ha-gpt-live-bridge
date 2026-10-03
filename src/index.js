import "dotenv/config";
import fs from "node:fs";
let addonOptions={}; try { addonOptions=JSON.parse(fs.readFileSync("/data/options.json","utf8")); } catch {}
import { ESP32WebSocketServer } from "./ESP32WebSocketServer.js";
import { GPTLiveClient } from "./GPTLiveClient.js";
const opts=addonOptions;
const gpt=new GPTLiveClient({apiKey:opts.vercel_api_key||process.env.VERCEL_API_KEY,model:opts.model||"openai/gpt-live-1",voice:opts.voice||"alloy",instructions:opts.instructions||"You are Jarvis.",haUrl:opts.ha_url||process.env.HA_URL,haToken:opts.ha_token||process.env.HA_TOKEN});
const server=new ESP32WebSocketServer({port:Number(process.env.PORT||8090),onAudio:(b)=>gpt.sendAudio(b),onOpen:()=>gpt.connect(),onClose:()=>gpt.close(),onControl:(m)=>gpt.handleControl(m)});
gpt.on("audio",b=>server.broadcastAudio(b)); gpt.on("event",e=>server.broadcast(e));
server.start(); console.log("GPT Live bridge listening on",server.port);
