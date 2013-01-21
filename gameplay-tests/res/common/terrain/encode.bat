@echo off

set encoder="..\..\..\..\bin\windows\gameplay-encoder.exe"

REM ====================================================================
REM Generate sky model
REM ====================================================================
%encoder% sky.fbx

REM ====================================================================
REM Generate normal map for terrain.
REM NOTE: Encoder currently expects a .raw format, so temporarily rename
REM .r16 to .raw for normal map generation.
REM ====================================================================
copy /Y heightmap.r16 heightmap.raw
%encoder% -n -w 10000,4000,10000 -s 256,256 heightmap.raw normalmap.png
del /Q heightmap.raw

pause
