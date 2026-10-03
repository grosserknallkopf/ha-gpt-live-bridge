#!/bin/sh
set -e

echo "[run.sh] Starting GPT Live Bridge..."

cd /app

# Options are provided by Supervisor as environment variables
export VERCEL_API_KEY="${vercel_api_key:-}"
export HA_URL="${ha_url:-http://homeassistant.local:8123}"
export HA_TOKEN="${ha_token:-}"
export VOICE="${voice:-verse}"
export PORT="${bridge_port:-8090}"
export LOG_LEVEL="${log_level:-info}"
export MODEL="${model:-openai/gpt-live-1}"
export INSTRUCTIONS="${instructions:-You are a helpful Home Assistant voice assistant.}"
export DELEGATION_MODEL="${delegation_model:-}"
export ALLOWED_HA_DOMAINS="${allowed_ha_domains:-light,switch,climate,media_player}"
export ENABLE_AEC="${enable_aec:-true}"
export NOISE_SUPPRESSION_LEVEL="${noise_suppression_level:-3}"

if [ -z "$VERCEL_API_KEY" ]; then
  echo "[run.sh] WARNING: vercel_api_key is not set!"
fi

# Start the server (CommonJS entrypoint, no build step)
if [ -f /app/server/server.js ]; then
  exec node /app/server/server.js
fi

# Fallback: try common entrypoints
echo "[run.sh] /app/server/server.js not found, listing /app/server:"
ls -la /app/server || true
exec node /app/server/index.js 2>/dev/null || node /app/src/server.js
