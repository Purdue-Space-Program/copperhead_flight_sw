FROM ubuntu:latest

WORKDIR /opt

RUN apt-get update -y
RUN apt-get install -y sudo git cmake
RUN sudo apt-get install -y ninja-build gcc-arm-none-eabi clang-format clang-tidy gcc g++
RUN sudo apt-get install -y build-essential libc++-dev libc++abi-dev
RUN sudo apt-get install -y libnewlib-arm-none-eabi
