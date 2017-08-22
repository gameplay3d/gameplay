set INPUT_FILE=%1
set OUTPUT_DIR=%2
%VULKAN_SDK%\Bin\glslangValidator.exe -V .\glsl\%INPUT_FILE% -o %OUTPUT_DIR%\%INPUT_FILE%.spv