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

echo Downloading %filename%.zip from %prefix%
%~d0
cd %~dp0
powershell -Command "([Net.ServicePointManager]::SecurityProtocol = \"tls12, tls11, tls\") -AND (Invoke-WebRequest %prefix%/%filename%.zip -OutFile %filename%.zip) | Out-Null"
goto :EXTRACT

:EXTRACT
echo Extracting %filename%.zip... please standby...
%~d0
cd %~dp0
> temp2.vbs ECHO Dim fileName, workingDir
>> temp2.vbs ECHO fileName = WScript.Arguments(0)
>> temp2.vbs ECHO workingDir = CreateObject("Scripting.FileSystemObject").GetAbsolutePathName(".")
>> temp2.vbs ECHO Set objShell = CreateObject("Shell.Application")
>> temp2.vbs ECHO Set objSource = objShell.NameSpace(workingDir ^& "\" ^& fileName).Items()
>> temp2.vbs ECHO Set objTarget = objShell.NameSpace(workingDir ^& "\")
>> temp2.vbs ECHO intOptions = 256
>> temp2.vbs ECHO objTarget.CopyHere objSource, intOptions
cscript temp2.vbs %filename%.zip
echo Cleaning up...
del temp2.vbs
del %filename%.zip
echo Done.
