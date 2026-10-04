ARG BUILD_FROM=ghcr.io/home-assistant/amd64-base-nodejs:latest
FROM ${BUILD_FROM}

WORKDIR /app

# Build-Kontext ist nur der Unterordner; der Rest des Repos wird beim Build geklont
RUN apk add --no-cache git \
    && git clone --depth 1 https://github.com/grosserknallkopf/ha-gpt-live-bridge /repo \
    && cp -r /repo/server /app/server \
    && cp -r /repo/src /app/src \
    && cp /repo/package.json /app/package.json \
    && rm -rf /repo

COPY run.sh /
RUN chmod a+x /run.sh \
    && cd /app && npm install --omit=dev --no-audit --no-fund

EXPOSE 8090

CMD ["/run.sh"]
