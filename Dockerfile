ARG BUILD_FROM=ghcr.io/home-assistant/amd64-base:3.19
FROM $BUILD_FROM

# Install Node.js + npm (HA base images are Alpine-based)
RUN apk add --no-cache nodejs npm

WORKDIR /app

# Copy the whole repo so server/, package.json etc. are all present
COPY . /app/

# npm ci requires package-lock.json which is not committed -> use npm install
RUN npm install --omit=dev --no-audit --no-fund || npm install --no-audit --no-fund

RUN chmod a+x /app/run.sh && cp /app/run.sh /run.sh && chmod a+x /run.sh

# config.yaml at /app so the build label maps correctly
# Entrypoint is managed by Home Assistant Supervisor (runs /run.sh)

CMD ["/run.sh"]
