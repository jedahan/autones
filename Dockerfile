FROM debian:latest

MAINTAINER Jonathan Dahan jonathan@jonathan.is

RUN apt-get update && apt-get upgrade -y && apt-get install -y fceux xvfb

VOLUME /var/tmp

CMD bash
