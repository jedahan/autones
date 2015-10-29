FROM debian/latest

MAINTAINER Jonathan Dahan jonathan@jonathan.is

COPY scripts/movie_to_autones.lua /usr/local/share

RUN apt-get install -y make gcc fceux

VOLUME /var/tmp

CMD bash
