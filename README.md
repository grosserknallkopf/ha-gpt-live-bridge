# Home Assistant GPT Live Bridge

ESP32 PCM WebSocket bridge to GPT Live 1 through Vercel AI Gateway. The bridge deliberately contains no secrets.

## Install
Install via **Settings → Add-ons → Add-on Store → ⋮ → Repositories**, add your GitHub repository URL, then install **GPT Live Bridge**. For a local checkout, copy this repository into `/addons/gpt_live_bridge`, add it as a local add-on, enter `vercel_api_key`, `ha_url`, and a long-lived `ha_token` in Configuration, then install/start. Expose TCP port 8090 to the LAN and set the ESPHome `server` URL to `ws://<HA-IP>:8090`.

## Protocol
ESP sends `{type:"audio-input",audio:"<base64 PCM>"}`; bridge returns `{type:"audio-output",audio:"<base64 PCM>"}`. Control events use `function-call` and `function-call-result`. Wake-word remains on-device; the ESP fallback pipeline remains available.

## Caveat
The Vercel realtime SDK surface is version-sensitive. Pin/test the exact `@ai-sdk/openai` version in the target deployment and confirm event names against the current Gateway Live API before production audio use.
