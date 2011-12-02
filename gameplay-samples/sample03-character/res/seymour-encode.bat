@echo off

REM This script converts seymour.dae to a gameplay binary format (.gpb) and groups all animations
REM targetting nodes/joints under 'group1' into a single animation called 'movements'.

"../../../gameplay-encoder/Debug/gameplay-encoder.exe" -groupAnimations group1 movements seymour.dae

pause
