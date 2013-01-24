#define LIGHTING

// Attributes
attribute vec4 a_position;									// Vertex position							(x, y, z, w)
attribute vec3 a_normal;									// Vertex normal							(x, y, z)
attribute vec2 a_texCoord;									// Vertex texture coordinate				(u, v)
#if defined(SKINNING)
attribute vec4 a_blendWeights;								// Vertex blend weight, up to 4				(0, 1, 2, 3) 
attribute vec4 a_blendIndices;								// Vertex blend index int u_matrixPalette	(0, 1, 2, 3)
#endif

// Uniforms
uniform mat4 u_worldViewProjectionMatrix;					// Matrix to transform a position to clip space
uniform mat4 u_inverseTransposeWorldViewMatrix;				// Matrix to transform a normal to view space
#if defined(SPECULAR) || defined(SPOT_LIGHT) || defined(POINT_LIGHT)
uniform mat4 u_worldViewMatrix;								// Matrix to tranform a position to view space
#endif
#if defined(SKINNING)
uniform vec4 u_matrixPalette[SKINNING_JOINT_COUNT * 3];		// Array of 4x3 matrices
#endif
#if defined(SPECULAR)
uniform vec3 u_cameraPosition;                 				// Position of the camera in view space
#endif
#if defined(TEXTURE_REPEAT)
uniform vec2 u_textureRepeat;								// Texture repeat for tiling
#endif
#if defined(TEXTURE_OFFSET)
uniform vec2 u_textureOffset;								// Texture offset
#endif
#if defined(POINT_LIGHT)
uniform vec3 u_pointLightPosition;							// Position of light
uniform float u_pointLightRangeInverse;						// Inverse of light range 
#elif defined(SPOT_LIGHT)
uniform vec3 u_spotLightPosition;							// Position of light
uniform float u_spotLightRangeInverse;						// Inverse of light range
uniform vec3 u_spotLightDirection;                          // Direction of a spot light source
#else
#endif

// Varyings
varying vec3 v_normalVector;								// Normal vector in view space
varying vec2 v_texCoord;									// Texture coordinate
#if defined(SPECULAR)
varying vec3 v_cameraDirection;								// Direction the camera is looking at in tangent space
#endif
#if defined(POINT_LIGHT)
varying vec3 v_vertexToPointLightDirection;					// Direction of point light w.r.t current vertex in tangent space
varying float v_pointLightAttenuation;						// Attenuation of point light
#include "lighting-point.vert"
#elif defined(SPOT_LIGHT)
varying vec3 v_vertexToSpotLightDirection;					// Direction of the spot light w.r.t current vertex in tangent space
varying float v_spotLightAttenuation;						// Attenuation of spot light

// Lighting
#include "lighting-spot.vert"
#else
#include "lighting-directional.vert"
#endif

// Skinning
#if defined(SKINNING)
#include "skinning.vert"
#else
#include "skinning-none.vert" 
#endif


void main()
{
    // Get the position and normal
    vec4 position = getPosition();
    vec3 normal = getNormal();

    // Transform position to clip space.
    gl_Position = u_worldViewProjectionMatrix * position;

    // Transform normal to view space.
	mat3 normalMatrix = mat3(u_inverseTransposeWorldViewMatrix[0].xyz, u_inverseTransposeWorldViewMatrix[1].xyz, u_inverseTransposeWorldViewMatrix[2].xyz);
    v_normalVector = normalMatrix * normal;

    // Apply light.
    applyLight(position);

    // Texture transformation
    v_texCoord = a_texCoord;
    #if defined(TEXTURE_REPEAT)
    v_texCoord *= u_textureRepeat;
    #endif
    #if defined(TEXTURE_OFFSET)
    v_texCoord += u_textureOffset;
    #endif
}
