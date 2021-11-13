FROM ubuntu:20.04

RUN DEBIAN_FRONTEND=noninteractive apt -y update
RUN DEBIAN_FRONTEND=noninteractive apt -y upgrade

RUN DEBIAN_FRONTEND=noninteractive apt -y install \
    git \
    build-essential \
    cmake \
    clang \
    doxygen \
    llvm \
    gdb \
    lldb \
    libboost-all-dev \
    libgtest-dev \
    libgmock-dev
