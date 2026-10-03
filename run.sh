#!/usr/bin/with-contenv bashio
set -e

export VERCEL_API_KEY=$(bashio::config 'vercel_api_key')
export HA_URL=$(bashio::config 'ha_url')
export HA_TOKEN=$(bashio::config 'ha_token')
export BRIDGE_PORT=$(bashio::config 'bridge_port')
export LOG_LEVEL=$(bashio::config 'log_level')
export MODEL=$(bashio::config 'model')
export VOICE=$(bashio::config 'voice')
export INSTRUCTIONS=$(bashio::config 'instructions')
export DELEGATION_MODEL=$(bashio::config 'delegation_model')
export ALLOWED_HA_DOMAINS=$(bashio::config 'allowed_ha_domains')
export ENABLE_AEC=$(bashio::config 'enable_aec')
export NOISE_SUPPRESSION_LEVEL=$(bashio::config 'noise_suppression_level')

echo "[GPT-Live Bridge] Starting on port ${BRIDGE_PORT}..."
cd /app
exec node server/server.js
