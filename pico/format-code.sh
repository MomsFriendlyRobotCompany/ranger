#!/bin/zsh

STYLE=${HOME}/github/dotfiles/clang-format.yml
clang-format -style=file:${STYLE} -i dev/*.cpp src/*.hpp src/*.cpp