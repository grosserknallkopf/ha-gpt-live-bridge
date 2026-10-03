ARG BUILD_FROM
FROM $BUILD_FROM

# Node.js installieren (Alpine-basiertes HA-Base-Image)
RUN apk add --no-cache nodejs npm \
    && mkdir -p /app

WORKDIR /app

COPY package.json /app/package.json
RUN npm install --omit=dev --no-audit --no-fund

COPY server/ /app/server/
COPY run.sh /run.sh
COPY run.sh /app/run.sh
RUN chmod a+x /run.sh /app/run.sh

# WICHTIG: KEIN ENTRYPOINT! Das HA-Base-Image (s6-overlay) muss PID 1 bleiben.
# Das Startkommando wird über S6_SERVICES / run.sh als Service gestartet.
CMD ["/run.sh"]
