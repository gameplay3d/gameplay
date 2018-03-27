#version 150
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 uPosition;
layout (location = 1) in vec3 uColor0;

layout (location = 0) out vec3 vColor0;

void main( void )
{
    gl_Position = vec4(uPosition.xyz, 1.0);
    vColor0 = uColor0;
}