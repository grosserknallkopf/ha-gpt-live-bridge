# GPT Live Bridge – korrigiertes Dockerfile
# Fix: /run.sh wurde nicht ins Image kopiert (Fehler: /run.sh: not found)

COPY run.sh /
RUN chmod a+x /run.sh

CMD ["/run.sh"]
