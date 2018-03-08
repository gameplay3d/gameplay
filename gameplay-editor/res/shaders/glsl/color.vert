#version 150
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;

layout (location = 0) out vec3 Color;

void main( void )
{
    gl_Position = vec4(vPosition.xyz, 1.0);
    Color = vColor;
}