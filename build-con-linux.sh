#!/usr/bin/env bash

mkdir -p ./bin
clang++ -o ./bin/console "$PWD"/src/console.cpp -O3 -std=c++20 -Wall -I"$PWD"/src -I"$PWD"/lib/shfl -lX11 -lGL -lpthread -lm -lstdc++
