FROM debian:bookworm

RUN apt update &&apt install -y \
     g++ \
     make \
     cmake \
     git \
     libssl-dev

