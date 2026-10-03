ARG BUILD_FROM
FROM $BUILD_FROM

WORKDIR /usr/src/app

COPY package.json package-lock.json* ./
RUN npm install --omit=dev || npm install

COPY server/ ./server/
COPY src/ ./src/
COPY custom_components/ ./custom_components/
COPY esphome/ ./esphome/

COPY run.sh /run.sh
RUN chmod a+x /run.sh && sed -i -e 's/\r$//' /run.sh

CMD ["/run.sh"]
