FROM node:20-alpine

WORKDIR /app

COPY server/package.json ./server/package.json
RUN cd server && npm install --omit=dev

COPY server ./server
COPY run.sh ./run.sh
RUN chmod +x ./run.sh

EXPOSE 8090

CMD ["/app/run.sh"]
