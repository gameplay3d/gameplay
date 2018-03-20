#!/bin/bash

if [ ! -d "spirv" ]; then mkdir -p "spirv"; fi

glslangValidator -V ./glsl/$1 -o ./spirv/$1.spv

