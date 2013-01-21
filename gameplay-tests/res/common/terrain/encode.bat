@echo off

set encoder="..\..\..\..\bin\windows\gameplay-encoder.exe"

%encoder% sky.fbx

%encoder% -n -w 10000,4000,10000 -s 256,256 heightmap.raw

pause
