FROM debian:bookworm

RUN apt update && apt install -y \
     g++ \
     make \
     cmake \
     git \
     curl \
     zip \
     unzip \
     tar \
     ninja-build \
     libssl-dev

RUN git clone https://github.com/garyhouston/rxspencer.git
RUN cd rxspencer && cmake . && make && make install

