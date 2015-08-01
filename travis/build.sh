#!/usr/bin/env bash

set -e
set -o pipefail

if [[ ${PLATFORM} == "osx" ]]; then
    echo "Building osx project"
    mkdir build && cd build
    cmake ..
    make
fi

if [[ ${PLATFORM} == "linux" ]]; then
    echo "Building linux project"
    mkdir build && cd build
    cmake ..
    make
fi
