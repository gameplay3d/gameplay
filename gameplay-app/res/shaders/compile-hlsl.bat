set URL=%1
set SHADER=%URL:~-4%
if "%SHADER%" == "vert" goto vsProfile
if "%SHADER%" == "tesc" goto hsProfile
if "%SHADER%" == "tese" goto dsProfile
if "%SHADER%" == "geom" goto gsProfile
if "%SHADER%" == "frag" goto psProfile
if "%SHADER%" == "comp" goto csProfile

set PROFILE=

:vsProfile
set PROFILE=vs_5_1
goto compile

:hsProfile
set PROFILE=hs_5_1
goto compile

:dsProfile
set PROFILE=ds_5_1
goto compile

:gsProfile
set PROFILE=gs_5_1
goto compile

:psProfile
set PROFILE=ps_5_1
goto compile

:csProfile
set PROFILE=cs_5_1
goto compile

:compile
"C:\Program Files (x86)\Windows Kits\10\bin\x64\fxc.exe" /T %PROFILE% /Fo .\%URL%.cso .\%URL%