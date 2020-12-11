FROM ubuntu:20.04

# system basics
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && \
  apt-get -y --no-install-recommends install \
    git \
    cmake \
    liblmdb-dev \
    nano && \
  apt-get clean && \
  rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

WORKDIR /home
RUN git config --global http.sslverify false
RUN git clone https://github.com/Maithem/dbe.git
WORKDIR /home/dbe
RUN cmake . && make
