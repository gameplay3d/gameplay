#define LIGHTING

uniform vec3 u_lightDirection;					// Light direction.

#if defined(SPECULAR)

uniform mat4 u_worldViewMatrix;                 // Matrix to tranform a position to view space.
uniform vec3 u_cameraPosition;                 	// Position of the camera in view space.
varying vec3 v_cameraDirection;                 // Direction the camera is looking at in tangent space.

#endif