#!/usr/bin/env bash

mkdir -p ./bin
clang++ -o ./bin/ray "$PWD"/src/ray.cpp -O3 -std=c++20 -Wall -I"$PWD"/src -I"$PWD"/lib/shfl -lX11 -lGL -lpthread -lm -lstdc++ -I"$PWD"/lib/raylib-5.5_linux_amd64/include/ -L"$PWD"/lib/raylib-5.5_linux_amd64/lib/ -l:libraylib.a
