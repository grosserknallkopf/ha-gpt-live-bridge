ARG BUILD_ARCH=amd64
FROM ghcr.io/home-assistant/${BUILD_ARCH}-base:3.19

WORKDIR /app

# The HA add-on base image invokes /app/run.sh as its entrypoint.
RUN apk add --no-cache nodejs npm jq

COPY server/package.json /app/server/package.json
RUN cd /app/server && npm install --omit=dev
COPY server/ /app/server/
COPY run.sh /app/run.sh
RUN chmod 0755 /app/run.sh

EXPOSE 8090
