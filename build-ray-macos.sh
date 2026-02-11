#!/usr/bin/env bash

MACOSX_DEPLOYMENT_TARGET="$(sw_vers -productVersion 2> /dev/null)"
mkdir -p ./bin
g++ -o ./bin/ray "$PWD"/src/ray.cpp -O3 -std=c++20 -mmacosx-version-min="$MACOSX_DEPLOYMENT_TARGET" -Wall -framework OpenGL -framework GLUT -framework Carbon -I"$PWD"/src -I"$PWD"/lib/shfl -I"$(brew --prefix)"/include -L"$(brew --prefix)"/lib -I"$PWD"/lib/raylib-5.5_macos/include -L"$PWD"/lib/raylib-5.5_macos/lib -rpath "$PWD"/lib/raylib-5.5_macos -lraylib
