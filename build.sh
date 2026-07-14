#!/usr/bin/env bash

cmake -B build \
    && cmake --build build --parallel $(($(nproc) - 1)) \
    && ctest --test-dir build 
