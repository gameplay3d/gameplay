#!/bin/bash


echo "Compiling glsl shaders to spirv..."

./compile-glsl.sh color.vert
./compile-glsl.sh color.frag
