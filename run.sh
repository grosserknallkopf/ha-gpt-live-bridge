#!/usr/bin/withcontenvinit
# Fallback-Start: wird normal über s6-Service gestartet,
# hier als direktes Skript lauffähig.
set -e

echo "[gpt-live-bridge] Starting bridge..."

export HA_URL="${HA_URL:-http://homeassistant.local:8123}"
export HA_TOKEN="${HA_TOKEN:-}"
export VERCEL_API_KEY="${VERCEL_API_KEY:-}"
export BRIDGE_PORT="${BRIDGE_PORT:-8090}"
export MODEL="${MODEL:-openai/gpt-live-1}"
export VOICE="${VOICE:-verse}"
export INSTRUCTIONS="${INSTRUCTIONS:-You are a helpful Home Assistant voice assistant.}"
export LOG_LEVEL="${LOG_LEVEL:-info}"

exec node /app/server/server.js
