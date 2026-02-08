#!/usr/bin/env bash

MACOSX_DEPLOYMENT_TARGET="$(sw_vers -productVersion 2> /dev/null)"
mkdir -p ./bin
clang++ -o ./bin/console "$PWD"/src/console.cpp -O3 -std=c++20 -mmacosx-version-min="$MACOSX_DEPLOYMENT_TARGET" -Wall -framework OpenGL -framework GLUT -framework Carbon -I"$PWD"/src -I"$PWD"/lib/shfl -I"$(brew --prefix)"/include -L"$(brew --prefix)"/lib
