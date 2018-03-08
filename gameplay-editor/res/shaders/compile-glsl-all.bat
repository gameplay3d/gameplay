@echo off

rem Compiling glsl shaders to spirv...

call compile-glsl color.vert
call compile-glsl color.frag
