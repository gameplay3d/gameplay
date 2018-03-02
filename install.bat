@echo off
REM
REM install.bat
REM
REM Download GamePlay dependencies and binaries from github releases and extracts from ZIP
REM
REM Helps prevent repo bloat due to large binary files
REM

set prefix=https://github.com/gameplay3d/GamePlay/releases/download/v4.0.0
set filename=gameplay-deps-windows
set zipfile=%filename%.zip

echo Downloading %prefix%%zipfile%...
powershell -Command "([Net.ServicePointManager]::SecurityProtocol= \"tls12, tls11, tls\") -AND ($ProgressPreference = 'SilentlyContinue') -AND(Invoke-WebRequest %prefix%/%filename%.zip -OutFile %filename%.zip) | Out-Null"

echo Extracting %filename%.zip... please standby...
powershell -Command "Expand-Archive %filename%.zip -Force" 
powershell -Command "Move-Item %filename%/external-deps -Destination "external-deps" -Force"

echo Cleaning up...
powershell -Command "Remove-Item %filename% -Force"
powershell -Command "Remove-Item %filename%.zip -Force"

echo Done.
