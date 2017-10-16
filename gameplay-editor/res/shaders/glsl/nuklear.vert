#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (binding = 0) uniform UBO 
{
    uniform mat4 projectionMatrix;
} ubo;

layout (location = 0) in vec2 vertPosition;
layout (location = 1) in vec2 vertTexCoord;
layout (location = 2) in vec4 vertColor;

layout (location = 0) out vec2 fragTexCoord;
layout (location = 1) out vec4 fragColor;


void main()
{
    gl_Position = ubo.projectionMatrix * vec4(vertPosition.xy, 0, 1);
    fragColor = vertColor;
    fragTexCoord = vertTexCoord;
}
