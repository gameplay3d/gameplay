@echo off

REM This script converts scene.dae to a gameplay binary format (.gpb) and groups all animations
REM targeting nodes/joints under 'HIKCharacterNode_Boy_Reference' the specified id into a single 
REM animation called 'animations'.

"..\..\..\..\bin\windows\gameplay-encoder.exe" -g HIKCharacterNode_Boy_Reference animations scene.dae ../common/scene.gpb

pause
