@echo off

REM This script converts scene.dae to a gameplay binary format (.gpb) and groups all animations
REM targetting nodes/joints under the specified id into a single animation called 'movements'.

"../../../gameplay-encoder/Debug/gameplay-encoder.exe" -groupAnimations HIKCharacterNode_Boy_Reference movements scene.dae

pause
