// Uniforms
uniform mat4 u_worldViewProjectionMatrix;       // Matrix to transform a position to clip space.
uniform mat4 u_inverseTransposeWorldViewMatrix; // Matrix to transform a normal to view space.
uniform mat4 u_worldMatrix;                     // Matrix to tranform a position to world space.
uniform mat4 u_worldViewMatrix;                 // Matrix to tranform a position to view space.
uniform vec3 u_cameraPosition;                  // Position of the camera.

// Inputs
attribute vec4 a_position;                      // Vertex Position (x, y, z, w)
attribute vec3 a_normal;                        // Vertex Normal (x, y, z)
attribute vec2 a_texCoord;                      // Vertex Texture Coordinate (u, v)
attribute vec3 a_tangent;                       // Vertex Tangent (x, y, z)
attribute vec3 a_binormal;                      // Vertex Binormal (actually Bi-tangent) (x, y, z)

// Outputs
varying vec2 v_texCoord;                        // Texture Coordinate (u,v)
varying vec3 v_cameraDirection;                 // Direction the camera is looking at in tangent space.

#if defined(POINT_LIGHT)

uniform vec3 u_pointLightPosition;              // Position
uniform float u_pointLightRadius;               // Radius 
varying vec3 v_vertexToPointLightDirection;     // Direction of point light w.r.t current vertex in tangent space.
varying float v_pointLightAttenuation;          // Attenuation of point light.

void applyLight(mat3 tangentSpaceTransformMatrix)
{
    // World space position.
    vec4 positionWorldViewSpace = u_worldViewMatrix * a_position;
    
    // Compute the light direction with light position and the vertex position.
    vec3 lightDirection = u_pointLightPosition - positionWorldViewSpace.xyz;
    
    // Transform current light direction to tangent space.
    vec3 vertexToPointLightDirection = tangentSpaceTransformMatrix * lightDirection;

    // Attenuation
    v_pointLightAttenuation = 1 - dot (lightDirection * u_pointLightRadius, lightDirection * u_pointLightRadius);

    // Output light direction.
    v_vertexToPointLightDirection =  vertexToPointLightDirection;
}

#elif defined(SPOT_LIGHT)

uniform vec3 u_spotLightPosition;               // Position
uniform vec3 u_spotLightDirection;              // Direction
varying vec3 v_spotLightDirection;              // Direction of spot light in tangent space.
varying vec3 v_vertexToSpotLightDirection;      // Direction of the spot light w.r.t current vertex in tangent space.

void applyLight(mat3 tangentSpaceTransformMatrix)
{
    // World space position.
    vec4 positionWorldViewSpace = u_worldViewMatrix * a_position;

    // Transform spot light direction to tangent space.
    v_spotLightDirection = tangentSpaceTransformMatrix * u_spotLightDirection;

    // Compute the light direction with light position and the vertex position.
    vec3 lightDirection = u_spotLightPosition - positionWorldViewSpace.xyz;
    
    // Transform current light direction to tangent space.
    lightDirection = tangentSpaceTransformMatrix * lightDirection;

    // Output light direction.
    v_vertexToSpotLightDirection = lightDirection;
}

#else

uniform vec3 u_lightDirection;                    // Direction
varying vec3 v_directionalLightDirection;         // Direction of light in tangent space.

void applyLight(mat3 tangentSpaceTransformMatrix)
{
    // Transform light direction to tangent space.
    v_directionalLightDirection = tangentSpaceTransformMatrix * u_lightDirection;
}

#endif

void main()
{
    // Transform position to clip space.
    gl_Position = u_worldViewProjectionMatrix * a_position;

    // Transform the normal, tangent and binormals to  view space.
    mat3 inverseTransposeWorldViewMatrix = mat3(u_inverseTransposeWorldViewMatrix);
    vec3 tangentVector  = inverseTransposeWorldViewMatrix * a_tangent;
    vec3 normalVector = inverseTransposeWorldViewMatrix * a_normal;
    vec3 binormalVector = inverseTransposeWorldViewMatrix * a_binormal;

    // Create a transform to convert a vector to tangent space.
    mat3 tangentSpaceTransformMatrix = mat3(tangentVector, binormalVector, normalVector);

    // Compute camera direction and transform it to tangent space.
    vec4 positionWorldSpace = u_worldMatrix * a_position;
    v_cameraDirection = tangentSpaceTransformMatrix * (u_cameraPosition - positionWorldSpace.xyz);

    // Apply light.
    applyLight(tangentSpaceTransformMatrix);

    // Pass on the texture coordinates to Fragment shader.
    v_texCoord = a_texCoord;
}