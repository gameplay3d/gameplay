
#ifndef JOINT_COUNT
#define JOINT_COUNT 75
#endif

const int c_zero = 0;
const int c_one = 1;
const int c_two = 2;
const int c_three = 3;

// Uniforms
uniform mat4 u_worldMatrix;
uniform mat4 u_worldViewProjectionMatrix;
uniform mat4 u_inverseTransposeWorldViewMatrix;
uniform vec3 u_cameraPosition;

// 32 4x3 matrices as an array of floats
uniform vec4 u_matrixPalette[JOINT_COUNT * 3];

// Inputs
attribute vec4 a_position;
attribute vec3 a_normal;
attribute vec2 a_texCoord;

attribute vec4 a_blendWeights;
attribute vec4 a_blendIndices;

// Outputs
varying vec2 v_texCoord;
varying vec3 v_normalVector;
varying vec3 v_cameraDirection;

void skin_position(in vec4 position, float m_wt, int m_indx, inout vec4 skinned_position)
{
    vec4 tmp;

    tmp.x = dot(position, u_matrixPalette[m_indx]);
    tmp.y = dot(position, u_matrixPalette[m_indx + c_one]);
    tmp.z = dot(position, u_matrixPalette[m_indx + c_two]);
    tmp.w = position.w;

    skinned_position += m_wt * tmp;
}

void skin_normal(in vec3 normal, float m_wt, int m_indx, inout vec3 skinned_normal)
{
    vec3 tmp;

    tmp.x = dot(normal, u_matrixPalette[m_indx].xyz);
    tmp.y = dot(normal, u_matrixPalette[m_indx + c_one].xyz);
    tmp.z = dot(normal, u_matrixPalette[m_indx + c_two].xyz);

    skinned_normal += m_wt * tmp;
}

void do_skinning(in vec4 position, in vec3 normal, out vec4 skinned_position, out vec3 skinned_normal)
{
    skinned_position = vec4(float(c_zero));
    skinned_normal = vec3(float(c_zero));

    // transform position and normal to eye space using matrix
    // palette with four matrices used to transform a vertex

    float m_wt = a_blendWeights[0];
    int m_indx = int (a_blendIndices[0]) * c_three;
    skin_position(position, m_wt, m_indx, skinned_position);
    skin_normal(normal, m_wt, m_indx, skinned_normal);

    m_wt = a_blendWeights[1];
    m_indx = int(a_blendIndices[1]) * c_three;
    skin_position(position, m_wt, m_indx, skinned_position);
    skin_normal(normal, m_wt, m_indx, skinned_normal);

    m_wt = a_blendWeights[2];
    m_indx = int(a_blendIndices[2]) * c_three;
    skin_position(position, m_wt, m_indx, skinned_position);
    skin_normal(normal, m_wt, m_indx, skinned_normal);

    m_wt = a_blendWeights[3];
    m_indx = int(a_blendIndices[2]) * c_three;
    skin_position(position, m_wt, m_indx, skinned_position);
    skin_normal(normal, m_wt, m_indx, skinned_normal);
}

void main()
{
    vec4 position;
    vec3 normal;
    do_skinning(a_position, a_normal, position, normal);
    
    gl_Position = u_worldViewProjectionMatrix * position;
    v_texCoord = a_texCoord;

    mat3 inverseTransposeWorldViewMatrix = mat3(u_inverseTransposeWorldViewMatrix[0].xyz, u_inverseTransposeWorldViewMatrix[1].xyz, u_inverseTransposeWorldViewMatrix[2].xyz);
    v_normalVector = inverseTransposeWorldViewMatrix * normal;

    vec4 positionWorldSpace = u_worldMatrix * position;
    v_cameraDirection = u_cameraPosition - positionWorldSpace.xyz;
}
