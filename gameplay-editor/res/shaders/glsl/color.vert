#version 150
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;

layout (location = 0) out vec3 vColor;

void main( void )
{
    gl_Position = vec4(inPosition.xyz, 1.0);
    vColor = inColor;
}