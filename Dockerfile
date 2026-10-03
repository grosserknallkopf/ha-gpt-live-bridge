ARG BUILD_FROM=ghcr.io/home-assistant/amd64-base-nodejs:latest
FROM ${BUILD_FROM}

WORKDIR /app

COPY run.sh /
COPY package.json /app/package.json
COPY server /app/server
COPY src /app/src

RUN chmod a+x /run.sh \
    && if [ -f /app/package.json ]; then cd /app && npm install --omit=dev --no-audit --no-fund; fi

EXPOSE 8090

CMD ["/run.sh"]
