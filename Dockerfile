FROM ubuntu:20.04

# system basics
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && \
  apt-get -y --no-install-recommends install \
    ca-certificates \
    build-essential \
    git \
    cmake \
    liblmdb-dev \
    clang-8 \
    libaio-dev \
    llvm \
    nano && \
  apt-get clean && \
  rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

WORKDIR /home
RUN git clone https://github.com/Maithem/dbe.git
WORKDIR /home
