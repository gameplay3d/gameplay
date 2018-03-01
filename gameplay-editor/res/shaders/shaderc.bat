@echo on

setlocal

echo Compiling shaders...
set working_dir=%~dp0
set shaderc_exe=%~dp0..\..\..\..\external-deps\bin\windows\shaderc.exe
set output_dir=%~dp0..\..\..\..\build\tools\editor\assets\shaders\d3d12\

if not exist "%output_dir%" mkdir "%output_dir%"

%shaderc_exe% -i %working_dir%common -f %working_dir%nuklear\nuklear.vert.sc -o %output_dir%nuklear.vert.bin --type vertex --platform windows -p vs_5_0 -O 3
%shaderc_exe% -i %working_dir%common -f %working_dir%nuklear\nuklear.frag.sc -o %output_dir%nuklear.frag.bin --type fragment --platform windows -p ps_5_0 -O 3

