require('dotenv').config();
const ESP32WebSocketServer=require('./ESP32WebSocketServer');
const config={apiKey:process.env.VERCEL_API_KEY,haUrl:process.env.HA_URL,haToken:process.env.HA_TOKEN,port:process.env.WEBSOCKET_PORT||8090,model:process.env.MODEL||'openai/gpt-live-1',voice:process.env.VOICE||'alloy',instructions:process.env.INSTRUCTIONS||'You are a helpful Home Assistant voice assistant.',delegationModel:process.env.DELEGATION_MODEL,allowedHaDomains:process.env.ALLOWED_HA_DOMAINS||'light,switch,climate,media_player,scene',enableAec:process.env.ENABLE_AEC!=='false',noiseSuppressionLevel:process.env.NOISE_SUPPRESSION_LEVEL||3};
if(!config.apiKey)console.warn('VERCEL_API_KEY is not set; sessions will fail until configured.');
const bridge=new ESP32WebSocketServer(config);bridge.start();process.on('SIGTERM',()=>{bridge.stop();process.exit(0)});process.on('SIGINT',()=>{bridge.stop();process.exit(0)});
