#version 150
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec3 vColor;

layout (location = 0) out vec3 Color;


void main()
{
    gl_Position = vPosition;
    Color = vColor;
}