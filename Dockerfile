ARG BUILD_FROM=ghcr.io/home-assistant/amd64-base:3.19
FROM $BUILD_FROM

# Node.js installieren
RUN apk add --no-cache nodejs npm

WORKDIR /app
COPY package.json ./
COPY server/ ./server/

# run.sh an BEIDEN Orten ablegen (HA-Supervisor ruft teils /run.sh, teils /app/run.sh)
COPY run.sh /run.sh
COPY run.sh /app/run.sh
RUN chmod a+x /run.sh /app/run.sh

EXPOSE 8090

# Kein ENTRYPOINT/CMD setzen — das HA-Base-Image startet /run.sh selbst
