#!/usr/bin/env bash

cmake -B build -DTOKENIZER_BUILD_TESTING=ON \
    && cmake --build build --parallel $(($(nproc) - 1)) \
    && ctest --test-dir build 
