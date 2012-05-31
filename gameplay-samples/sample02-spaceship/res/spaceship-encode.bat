@echo off

REM This script converts spaceship.dae to a gameplay binary format (.gpb).

"../../../gameplay-encoder/Debug/gameplay-encoder.exe" spaceship.dae

pause
