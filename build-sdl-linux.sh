#!/usr/bin/env bash

mkdir -p ./bin
cp ./assets/* ./bin/
clang++ -o ./bin/sdl "$PWD"/src/sdl.cpp -O3 -std=c++20 -Wall -I"$PWD"/src -I"$PWD"/lib/shfl -lX11 -lGL -lpthread -lm -lstdc++ -I"$SDL3_DIR"/include -lSDL3 -lSDL3_image -lSDL3_mixer -lSDL3_ttf -L"$SDL3_DIR"/lib
