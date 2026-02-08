#!/usr/bin/env bash

mkdir -p ./bin
cp ./assets/* ./bin/
clang++ -o ./bin/sdl "$PWD"/src/sdl.cpp -O3 -lstdc++fs -std=c++20 -I"$PWD"/src -I"$PWD"/lib/shfl -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lwinmm -I"$SDL3_DIR"/include -lSDL3 -lSDL3_image -lSDL3_mixer -lSDL3_ttf -L"$SDL3_DIR"/lib
