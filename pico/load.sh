#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Must give file name to load onto the pi pico"
    echo ">> ${0} <filename>"
    exit 1
fi

# UF2="$(ls *.uf2)"
UF2=$1

echo "Loading ${UF2} to pi pico"
picotool load "${UF2}" -v -f