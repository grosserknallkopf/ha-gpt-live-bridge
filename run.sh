#!/bin/sh
set -eu

# HA add-on options are exposed as lower-case variables. Preserve explicit
# upper-case overrides for local/container use, then start the server.
export VERCEL_API_KEY="${VERCEL_API_KEY:-${vercel_api_key:-}}"
export HA_URL="${HA_URL:-${ha_url:-http://homeassistant.local:8123}}"
export HA_TOKEN="${HA_TOKEN:-${ha_token:-}}"
export BRIDGE_PORT="${BRIDGE_PORT:-${bridge_port:-8090}}"
export MODEL="${MODEL:-${model:-openai/gpt-live-1}}"
export VOICE="${VOICE:-${voice:-alloy}}"
export INSTRUCTIONS="${INSTRUCTIONS:-${instructions:-You are a helpful Home Assistant voice assistant.}}"
export DELEGATION_MODEL="${DELEGATION_MODEL:-${delegation_model:-openai/gpt-4o-mini}}"
export ALLOWED_HA_DOMAINS="${ALLOWED_HA_DOMAINS:-${allowed_ha_domains:-light,switch,climate,media_player,scene}}"
export ENABLE_AEC="${ENABLE_AEC:-${enable_aec:-true}}"
export NOISE_SUPPRESSION_LEVEL="${NOISE_SUPPRESSION_LEVEL:-${noise_suppression_level:-3}}"

exec node /app/server/server.js
