@echo off

REM This script converts scene.dae to a gameplay binary format (.gpb) and groups all animations
REM targeting nodes/joints under 'HIKCharacterNode_Boy_Reference' the specified id into a single 
REM animation called 'movements'.

"../../../../gameplay-encoder/Debug/gameplay-encoder.exe" -g HIKCharacterNode_Boy_Reference movements scene.dae

pause
