ARG BUILD_ARCH=amd64
FROM ghcr.io/home-assistant/${BUILD_ARCH}-base:3.19

WORKDIR /app

# The HA add-on base image invokes /app/run.sh as its entrypoint.
RUN apk add --no-cache nodejs npm jq

COPY . /app/
RUN cd /app/server && npm install --omit=dev
RUN chmod a+x /app/run.sh

EXPOSE 8090
