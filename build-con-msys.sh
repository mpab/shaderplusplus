#!/usr/bin/env bash

mkdir -p ./bin
clang++ -o ./bin/console "$PWD"/src/console.cpp -O3 -lstdc++fs -std=c++20 -I"$PWD"/src -I"$PWD"/lib/shfl -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lwinmm
