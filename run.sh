#!/usr/bin/with-contenv bashio
set -e

export VERCEL_API_KEY=$(bashio::config 'vercel_api_key')
export HA_URL=$(bashio::config 'ha_url')
export HA_TOKEN=$(bashio::config 'ha_token')
export BRIDGE_PORT=$(bashio::config 'bridge_port')
export VOICE=$(bashio::config 'voice')
export LOG_LEVEL=$(bashio::config 'log_level')

bashio::log.info "Starting GPT Live Bridge on port ${BRIDGE_PORT}..."

cd /app
node server/server.js