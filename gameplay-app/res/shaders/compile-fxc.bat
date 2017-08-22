set INPUT_FILE=%1
set OUTPUT_DIR=%2
set PROFILE=%3
"C:\Program Files (x86)\Windows Kits\10\bin\x64\fxc.exe" /T %PROFILE% /Fo .\hlsl\%INPUT_FILE%.cso %OUTPUT_DIR%\%INPUT_FILE%