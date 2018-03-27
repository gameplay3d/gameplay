#version 150
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 vColor0;

layout (location = 0) out vec4 outColor0;

void main( void )
{
    outColor0 = vec4(vColor0.xyz, 1);
}