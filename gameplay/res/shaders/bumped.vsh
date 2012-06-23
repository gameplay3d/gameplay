// Uniforms
uniform mat4 u_worldViewProjectionMatrix;           // Matrix to transform a position to clip space
uniform mat4 u_inverseTransposeWorldViewMatrix;     // Matrix to transform a normal to view space
#if defined(TEXTURE_REPEAT)
uniform vec2 u_textureRepeat;
#endif
#if defined(TEXTURE_OFFSET)
uniform vec2 u_textureOffset;
#endif

// Inputs
attribute vec4 a_position;                          // Vertex Position              (x, y, z, w)
attribute vec3 a_normal;                            // Vertex Normal                (x, y, z)
attribute vec2 a_texCoord;                          // Vertex Texture Coordinate    (u, v)
attribute vec3 a_tangent;                           // Vertex Tangent               (x, y, z)
attribute vec3 a_binormal;                          // Vertex Binormal/Bitangent    (x, y, z)

// Outputs
varying vec2 v_texCoord;                            // Output Texture Coordinate     (u,v)

// Lighting
#include "lib/lighting.vsh"
#if defined(POINT_LIGHT)
#include "lib/lighting-point.vsh"
#elif defined(SPOT_LIGHT)
#include "lib/lighting-spot.vsh"
#else
#include "lib/lighting-directional.vsh"
#endif

// Attribute Accessors (getPosition(), getNormal(), etc.)
#define BUMPED
#if defined(SKINNING)
#include "lib/attributes-skinning.vsh"
#else
#include "lib/attributes.vsh" 
#endif

void main()
{
    // Get the position, normal, tangents and binormals.
    vec4 position = getPosition();
    vec3 normal = getNormal();
    vec3 tangent = getTangent();
    vec3 binormal = getBinormal();
    
    // Transform position to clip space.
    gl_Position = u_worldViewProjectionMatrix * position;

    // Transform the normal, tangent and binormals to view space.
    mat3 inverseTransposeWorldViewMatrix = mat3(u_inverseTransposeWorldViewMatrix[0].xyz, u_inverseTransposeWorldViewMatrix[1].xyz, u_inverseTransposeWorldViewMatrix[2].xyz);
    vec3 normalVector = normalize(inverseTransposeWorldViewMatrix * normal);
    
    // Create a transform to convert a vector to tangent space.
    vec3 tangentVector  = normalize(inverseTransposeWorldViewMatrix * tangent);
    vec3 binormalVector = normalize(inverseTransposeWorldViewMatrix * binormal);
    mat3 tangentSpaceTransformMatrix = mat3(tangentVector.x, binormalVector.x, normalVector.x, tangentVector.y, binormalVector.y, normalVector.y, tangentVector.z, binormalVector.z, normalVector.z);
    
    // Apply light.
    applyLight(tangentSpaceTransformMatrix);
    
    // Texture transformation.
    v_texCoord = a_texCoord;
    #if defined(TEXTURE_REPEAT)
    v_texCoord *= u_textureRepeat;
    #endif
    #if defined(TEXTURE_OFFSET)
    v_texCoord += u_textureOffset;
    #endif
}