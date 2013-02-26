#define LIGHTING

// Attributes
attribute vec4 a_position;									// Vertex Position							(x, y, z, w)
attribute vec3 a_normal;									// Vertex Normal							(x, y, z)
#if defined(SKINNING)
attribute vec4 a_blendWeights;								// Vertex blend weight, up to 4				(0, 1, 2, 3) 
attribute vec4 a_blendIndices;								// Vertex blend index int u_matrixPalette	(0, 1, 2, 3)
#endif
#if defined(VERTEX_COLOR)
attribute vec3 a_color;										// Output Vertex Color
varying vec3 v_color;										// Output Vertex Color 
#endif

// Uniforms
uniform mat4 u_worldViewProjectionMatrix;					// Matrix to transform a position to clip space.
uniform mat4 u_inverseTransposeWorldViewMatrix;				// Matrix to transform a normal to view space
uniform mat4 u_worldViewMatrix;								// Matrix to tranform a position to view space.
#if defined(SKINNING)
uniform vec4 u_matrixPalette[SKINNING_JOINT_COUNT * 3];		// Array of 4x3 matrices
#endif
#if defined(SPECULAR)
uniform vec3 u_cameraPosition;                 				// Position of the camera in view space.
#endif
#if defined(POINT_LIGHT)
uniform vec3 u_pointLightPosition;							// Position of light
uniform float u_pointLightRangeInverse;						// Inverse of light range 
#elif defined(SPOT_LIGHT)
uniform vec3 u_spotLightPosition;							// Position of light
uniform float u_spotLightRangeInverse;						// Inverse of light range.
#else
#endif

// Varyings
varying vec3 v_normalVector;								// Normal vector in view space.
#if defined(SPECULAR)
varying vec3 v_cameraDirection;								// Direction the camera is looking at in tangent space.
#endif

// Lighting
#if defined(POINT_LIGHT)
varying vec3 v_vertexToPointLightDirection;					// Direction of point light w.r.t current vertex in tangent space.
varying float v_pointLightAttenuation;						// Attenuation of point light.
#include "lighting-point.vert"
#elif defined(SPOT_LIGHT)
varying vec3 v_vertexToSpotLightDirection;					// Direction of the spot light w.r.t current vertex in tangent space.
varying float v_spotLightAttenuation;						// Attenuation of spot light.
#include "lighting-spot.vert"
#else
uniform vec3 u_lightDirection;								// Direction of light
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
    mat3 inverseTransposeWorldViewMatrix = mat3(u_inverseTransposeWorldViewMatrix[0].xyz, u_inverseTransposeWorldViewMatrix[1].xyz, u_inverseTransposeWorldViewMatrix[2].xyz);
    v_normalVector = inverseTransposeWorldViewMatrix * normal;

    // Apply light.
    applyLight(position);
    
    // Pass the vertex color to fragment shader
    #if defined(VERTEX_COLOR)
	v_color = a_color;
    #endif
}
