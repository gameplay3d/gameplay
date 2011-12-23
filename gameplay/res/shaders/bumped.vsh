// Uniforms
uniform mat4 u_worldViewProjectionMatrix;       // Matrix to transform a position to clip space.
uniform mat4 u_inverseTransposeWorldViewMatrix; // Matrix to transform a normal to view space.

// Inputs
attribute vec4 a_position;                      // Vertex Position (x, y, z, w)
attribute vec3 a_normal;                        // Vertex Normal (x, y, z)
attribute vec2 a_texCoord;                      // Vertex Texture Coordinate (u, v)
attribute vec3 a_tangent;                       // Vertex Tangent (x, y, z)
attribute vec3 a_binormal;                      // Vertex Binormal (actually Bi-tangent) (x, y, z)

// Outputs
varying vec2 v_texCoord;                        // Texture Coordinate (u,v)

#if defined(SKINNING)

attribute vec4 a_blendWeights;
attribute vec4 a_blendIndices;

// 32 4x3 matrices as an array of floats
uniform vec4 u_matrixPalette[SKINNING_JOINT_COUNT * 3];

// Common vectors.
vec4 _skinnedPosition;
vec3 _skinnedNormal;

void skinPosition(float blendWeight, int matrixIndex)
{
    vec4 tmp;

    tmp.x = dot(a_position, u_matrixPalette[matrixIndex]);
    tmp.y = dot(a_position, u_matrixPalette[matrixIndex + 1]);
    tmp.z = dot(a_position, u_matrixPalette[matrixIndex + 2]);
    tmp.w = a_position.w;

    _skinnedPosition += blendWeight * tmp;
}

vec4 getPosition()
{
    _skinnedPosition = vec4(0.0);

    // Transform position to view space using 
    // matrix palette with four matrices used to transform a vertex.

    float blendWeight = a_blendWeights[0];
    int matrixIndex = int (a_blendIndices[0]) * 3;
    skinPosition(blendWeight, matrixIndex);

    blendWeight = a_blendWeights[1];
    matrixIndex = int(a_blendIndices[1]) * 3;
    skinPosition(blendWeight, matrixIndex);

    blendWeight = a_blendWeights[2];
    matrixIndex = int(a_blendIndices[2]) * 3;
    skinPosition(blendWeight, matrixIndex);

    blendWeight = a_blendWeights[3];
    matrixIndex = int(a_blendIndices[3]) * 3;
    skinPosition(blendWeight, matrixIndex);

    return _skinnedPosition;    
}

void skinTangentSpaceVector(vec3 vector, float blendWeight, int matrixIndex)
{
    vec3 tmp;

    tmp.x = dot(vector, u_matrixPalette[matrixIndex].xyz);
    tmp.y = dot(vector, u_matrixPalette[matrixIndex + 1].xyz);
    tmp.z = dot(vector, u_matrixPalette[matrixIndex + 2].xyz);

    _skinnedNormal += blendWeight * tmp;
}

vec3 getTangentSpaceVector(vec3 vector)
{
    _skinnedNormal = vec3(0.0);

    // Transform normal to view space using 
    // matrix palette with four matrices used to transform a vertex.

    float blendWeight = a_blendWeights[0];
    int matrixIndex = int (a_blendIndices[0]) * 3;
    skinTangentSpaceVector(vector, blendWeight, matrixIndex);

    blendWeight = a_blendWeights[1];
    matrixIndex = int(a_blendIndices[1]) * 3;
    skinTangentSpaceVector(vector, blendWeight, matrixIndex);

    blendWeight = a_blendWeights[2];
    matrixIndex = int(a_blendIndices[2]) * 3;
    skinTangentSpaceVector(vector, blendWeight, matrixIndex);

    blendWeight = a_blendWeights[3];
    matrixIndex = int(a_blendIndices[3]) * 3;
    skinTangentSpaceVector(vector, blendWeight, matrixIndex);

    return _skinnedNormal;
}

vec3 getNormal()
{
    return getTangentSpaceVector(a_normal);
}

vec3 getTangent()
{
    return getTangentSpaceVector(a_tangent);
}

vec3 getBinormal()
{
    return getTangentSpaceVector(a_binormal);
}

#else

vec4 getPosition()
{
    return a_position;    
}

vec3 getNormal()
{
    return a_normal;
}

vec3 getTangent()
{
    return a_tangent;
}

vec3 getBinormal()
{
    return a_binormal;
}

#endif

#if defined(POINT_LIGHT)

uniform mat4 u_worldViewMatrix;                 // Matrix to tranform a position to view space.
uniform vec3 u_pointLightPosition;              // Position
uniform float u_pointLightRangeInverse;         // Inverse of light range.
varying vec3 v_vertexToPointLightDirection;     // Direction of point light w.r.t current vertex in tangent space.
varying float v_pointLightAttenuation;          // Attenuation of point light.

void applyLight(mat3 tangentSpaceTransformMatrix)
{
    // World space position.
    vec4 positionWorldViewSpace = u_worldViewMatrix * a_position;
    
    // Compute the light direction with light position and the vertex position.
    vec3 lightDirection = u_pointLightPosition - positionWorldViewSpace.xyz;
    
    // Transform current light direction to tangent space.
    vec3 vertexToSpotLightDirection = tangentSpaceTransformMatrix * lightDirection;

    // Attenuation
    v_pointLightAttenuation = 1.0 - dot(lightDirection * u_pointLightRangeInverse, lightDirection * u_pointLightRangeInverse);

    // Output light direction.
    v_vertexToPointLightDirection =  vertexToSpotLightDirection;
}

#elif defined(SPOT_LIGHT)

uniform mat4 u_worldViewMatrix;                 // Matrix to tranform a position to view space.
uniform vec3 u_spotLightPosition;               // Position
uniform float u_spotLightRangeInverse;          // Inverse of light range.
uniform vec3 u_spotLightDirection;              // Direction
varying vec3 v_spotLightDirection;              // Direction of spot light in tangent space.
varying vec3 v_vertexToSpotLightDirection;      // Direction of the spot light w.r.t current vertex in tangent space.
varying float v_spotLightAttenuation;           // Attenuation of spot light.

void applyLight(mat3 tangentSpaceTransformMatrix)
{
    vec4 positionWorldViewSpace = u_worldViewMatrix * a_position;

    // Transform spot light direction to tangent space.
    v_spotLightDirection = tangentSpaceTransformMatrix * u_spotLightDirection;

    // Compute the light direction with light position and the vertex position.
    vec3 lightDirection = u_spotLightPosition - positionWorldViewSpace.xyz;
    
    // Transform current light direction to tangent space.
    lightDirection = tangentSpaceTransformMatrix * lightDirection;
    
    // Attenuation
    v_spotLightAttenuation = 1.0 - dot(lightDirection * u_spotLightRangeInverse, lightDirection * u_spotLightRangeInverse);

    // Output light direction.
    v_vertexToSpotLightDirection = lightDirection;
}

#else

uniform vec3 u_lightDirection;                  // Direction
varying vec3 v_lightDirection;                  // Direction of light in tangent space.

void applyLight(mat3 tangentSpaceTransformMatrix)
{
    // Transform light direction to tangent space.
    v_lightDirection = tangentSpaceTransformMatrix * u_lightDirection;
}

#endif

void main()
{
    vec4 position = getPosition();
    vec3 normal = getNormal();
    vec3 tangent = getTangent();
    vec3 binormal = getBinormal();
    
    // Transform position to clip space.
    gl_Position = u_worldViewProjectionMatrix * position;

    // Transform the normal, tangent and binormals to  view space.
    mat3 inverseTransposeWorldViewMatrix = mat3(u_inverseTransposeWorldViewMatrix);
    vec3 tangentVector  = normalize(inverseTransposeWorldViewMatrix * tangent);
    vec3 normalVector = normalize(inverseTransposeWorldViewMatrix * normal);
    vec3 binormalVector = normalize(inverseTransposeWorldViewMatrix * binormal);

    // Create a transform to convert a vector to tangent space.
    mat3 tangentSpaceTransformMatrix = mat3(tangentVector.x, binormalVector.x, normalVector.x,
                                            tangentVector.y, binormalVector.y, normalVector.y,
                                            tangentVector.z, binormalVector.z, normalVector.z);

    // Apply light.
    applyLight(tangentSpaceTransformMatrix);

    // Pass on the texture coordinates to Fragment shader.
    v_texCoord = a_texCoord;
}