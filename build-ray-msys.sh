#!/usr/bin/env bash

mkdir -p ./bin
g++ -o ./bin/ray "$PWD"/src/ray.cpp -O3 -lstdc++fs -std=c++20 -I"$PWD"/src -I"$PWD"/lib/shfl -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lwinmm -I"$PWD"/lib/raylib-5.5_win64_mingw-w64/include/ -L"$PWD"/lib/raylib-5.5_win64_mingw-w64/lib/ -l:libraylib.a -lgdi32 -lwinmm
