// Uniforms
uniform    vec3 u_lightDirection;                    // Direction of the light
uniform mat4 u_worldViewProjectionMatrix;            // Matrix to transform a position to clip space.
uniform mat4 u_inverseTransposeWorldViewMatrix;      // Matrix to transform a normal to view space.
uniform mat4 u_worldMatrix;                          // Matrix to tranform a position to world space.
uniform vec3 u_cameraPosition;                       // Position of the camera.

// Inputs
attribute vec4 a_position;                           // Vertex Position (x, y, z, w)
attribute vec3 a_normal;                             // Vertex Normal (x, y, z)
attribute vec2 a_texCoord;                           // Vertex Texture Coordinate (u, v)
attribute vec3 a_tangent;                            // Vertex Tangent (x, y, z)
attribute vec3 a_binormal;                           // Vertex Binormal (actually Bi-tangent) (x, y, z)

// Outputs
varying vec2 v_texCoord;                             // Texture Coordinate (u,v)
varying vec3 v_lightDirectionTangentSpace;           // Direction of the light in tangent space.
varying vec3 v_cameraDirectionTangentSpace;          // Direction the camera is looking at in tangent space.

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

    // Transform light direction to tangent space.
    v_lightDirectionTangentSpace = tangentSpaceTransformMatrix * u_lightDirection;

    // Compute camera direction and transform it to tangent space.
    vec4 positionWorldSpace = u_worldMatrix * a_position;
    v_cameraDirectionTangentSpace = tangentSpaceTransformMatrix * (u_cameraPosition - positionWorldSpace.xyz);

    // Pass on the texture coordinates to Fragment shader.
    v_texCoord = a_texCoord;
}