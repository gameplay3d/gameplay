@echo off
setlocal EnableDelayedExpansion

set toolpath=C:\texturecompress\texturecompress.exe

echo.
echo Supported compression formats:
echo.
echo   1. DXTC
echo   2. PVRTC
echo   3. ATC
echo.

set /P tchoice=Enter format (1-3): 
echo.
if '%tchoice%' == '1' (
  set tpath=dxt
  set tformat=DXT5
  set text=.dds
  goto compress
)

if '%tchoice%' == '2' (
  set tpath=pvrtc
  set tformat=PVRTC4v1
  set text=.pvr
  goto compress
)

if '%tchoice%' == '3' (
  set tpath=atc
  set tformat=ATCA
  set text=.dds
  goto compress
)

echo ERROR: Invalid texture format choice.
echo.
goto end

:compress

dir /B *.png > textures.txt

if not exist ..\%tpath% (
	mkdir ..\%tpath%
)

echo aliases > ..\%tpath%\resource.aliases
echo { >> ..\%tpath%\resource.aliases

for /F "tokens=*" %%a in (textures.txt) do (
    "%toolpath%" -m -yflip -f%tformat% ".\%%a" "..\%tpath%\%%~na%text%"
    echo     %%~na = res/%tpath%/%%~na%text% >> ..\%tpath%\resource.aliases
)

echo } >> ..\%tpath%\resource.aliases

del /Q textures.txt

:end

echo.
pause
