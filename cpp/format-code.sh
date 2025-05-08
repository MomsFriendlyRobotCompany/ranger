#!/bin/zsh

STYLE=${HOME}/github/dotfiles/clang-format.yml
clang-format -style=file:${STYLE} -i src/*.h src/*.c src/*.hpp src/*.cpp