FROM ubuntu:22.10

RUN apt-get update && apt-get install -y \
    cmake \
    gcc-arm-none-eabi \
    libnewlib-arm-none-eabi \
    build-essential \
    git \
    libstdc++-arm-none-eabi-newlib \
    python3 \
    wget

RUN git clone https://github.com/raspberrypi/pico-sdk.git --branch master

RUN cd pico-sdk && git submodule update --init

RUN git clone https://github.com/raspberrypi/pico-examples.git --branch master

ENV PICO_SDK_PATH=/pico-sdk