FROM node:22-alpine

RUN apk add --no-cache bash

WORKDIR /app

COPY server/ /app/
COPY run.sh /app/run.sh
RUN chmod +x /app/run.sh

RUN npm install --production

EXPOSE 8090

CMD ["/app/run.sh"]