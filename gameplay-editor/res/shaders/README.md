# Compiling shaders

Cross-platform graphics api needs shaders and uses the following shaders extensions: 

.vert = Vertex shader
.tesc = Tesselation Control shader | Hull shader(hlsl)
.tece = Tesselation Evaluation shader | Domain shader(hlsl)
.geom = Geometry shader
.frag = Fragment shader | Pixel shader(hlsl)
.comp = Compute shader

## Compiling shaders using compile-xxxx.bat/.sh scripts

Below is a simple example to compile a vertex and fragment shader:

### Vulkan: (glsl -> .spv )
This compiles GLSL shaders using the Windows 10 SDK fxc.exe

(Windows)
compile-glsl.bat glsl\nulkear.vert    
compile-glsl.bat glsl\nulkear.frag

(Linux)
compile-glsl.sh glsl\nulkear.vert    
compile-glsl.sh glsl\nulkear.frag

### Direct3D12: Effects compiler (hlsl -> .cso )
This following script compiles HLSL shaders using the Windows 10 SDK fxc.exe

(Windows)
compile-hlsl.bat hlsl\nuklear.vert
compile-hlsl.bat hlsl\nuklear.frag

### Metal: (msl -> .metallib) 
(MacOS)
compile-msl.sh msl\nuklear.vert
compile-msl.sh msl\nuklear.frag


