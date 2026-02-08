#!/usr/bin/env bash

MACOSX_DEPLOYMENT_TARGET="$(sw_vers -productVersion 2> /dev/null)"
mkdir -p ./bin
cp ./assets/* ./bin/
clang++ -o ./bin/sdl "$PWD"/src/sdl.cpp -O3 -std=c++20 -mmacosx-version-min="$MACOSX_DEPLOYMENT_TARGET" -Wall -framework OpenGL -framework GLUT -framework Carbon -I"$PWD"/src -I"$PWD"/lib/shfl -I"$(brew --prefix)"/include -L"$(brew --prefix)"/lib -I"$SDL3_DIR"/include -lSDL3 -lSDL3_image -lSDL3_mixer -lSDL3_ttf -L"$SDL3_DIR"/lib -rpath "$SDL3_DIR"/lib
