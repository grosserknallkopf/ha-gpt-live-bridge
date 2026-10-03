# GPT Live Bridge

Home Assistant add-on and ESPHome component that bridges raw PCM audio from an ESP32 to GPT Live through the Vercel AI Gateway.

## Installation

1. Add this repository as a Home Assistant add-on repository.
2. Install **GPT Live Bridge** and configure `vercel_api_key`, `ha_url`, and `ha_token`.
3. Start the add-on. With host networking enabled, ESP32 devices can use `ws://homeassistant.local:8090`.
4. Copy `esphome/jarvis.yaml` and provide the referenced secrets.
5. Compile and flash with ESPHome.

The bridge accepts binary PCM WebSocket frames and JSON commands (`connect`, `disconnect`, `ping`). It returns binary response audio and JSON transcript/status events. Keep API keys and HA tokens in Home Assistant options or ESPHome secrets; never commit them.

## Local server

```sh
npm install
VERCEL_API_KEY=... HA_URL=http://homeassistant.local:8123 HA_TOKEN=... npm start
```
