@echo off
rem
rem setup.bat 
rem
rem download + install: premake-<platform>.zip -> _deps
set install_path=%cd%\_deps
set package_url=https://github.com/gameplay3d/gameplay-deps/releases/download/v4.0.0/
set filename=premake-5.0.0-windows
set zipfile=%filename%.zip
if not exist %install_path% mkdir %install_path%
echo Downloading %prefix%%zipfile%...
powershell -Command "([Net.ServicePointManager]::SecurityProtocol=\"tls12, tls11, tls\") -AND ($ProgressPreference='SilentlyContinue') -AND (Invoke-WebRequest '%package_url%/%filename%.zip' -OutFile '%filename%.zip') | Out-Null"
echo Extracting %filename%.zip... please standby...
powershell -Command "Expand-Archive %filename%.zip -Force -DestinationPath %install_path%" 
powershell -Command "Remove-Item %filename%.zip -Force"
rem download + install: gameplay-deps-<platform>.zip -> _deps
set package_url=https://github.com/gameplay3d/gameplay/releases/download/v4.0.0/
set filename=gameplay-deps-windows
set zipfile=%filename%.zip
if not exist %install_path% mkdir %install_path%
echo Downloading %prefix%%zipfile%...
powershell -Command "([Net.ServicePointManager]::SecurityProtocol=\"tls12, tls11, tls\") -AND ($ProgressPreference='SilentlyContinue') -AND (Invoke-WebRequest '%package_url%/%filename%.zip' -OutFile '%filename%.zip') | Out-Null"
echo Extracting %filename%.zip... please standby...
powershell -Command "Expand-Archive %filename%.zip -Force -DestinationPath %install_path%" 
powershell -Command "Remove-Item %filename%.zip -Force"
echo Done.
