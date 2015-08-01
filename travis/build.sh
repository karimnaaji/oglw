#!/usr/bin/env bash

set -e
set -o pipefail

if [[ ${PLATFORM} == "osx" ]]; then
    mkdir build && cd build
    cmake ..
    make
fi

if [[ ${PLATFORM} == "linux" ]]; then
    mkdir build && cd build
    cmake ..
    make
fi
