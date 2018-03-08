set URL=%1
if not exist "spirv" mkdir "spirv"
%VULKAN_SDK%\Bin\glslangValidator.exe -V .\glsl\%URL% -o .\spirv\%URL%.spv