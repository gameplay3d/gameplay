// Attributes
attribute vec4 a_position;									// Vertex Position							(x, y, z, w)
attribute vec3 a_normal;									// Vertex Normal							(x, y, z)

// Uniforms
uniform mat4 u_worldViewProjectionMatrix;					// Matrix to transform a position to clip space.
uniform mat4 u_inverseTransposeWorldViewMatrix;				// Matrix to transform a normal to view space
uniform mat4 u_worldMatrix;									// Matrix to tranform a position to world space.
uniform vec3 u_cameraPosition;                 				// Position of the camera in world space.

// Varyings
varying vec3 v_normalVector;								// Normal vector in view space
varying vec3 v_eyeVector;									// Eye vector in world space

void main()
{
    // Transform position to clip space.
    gl_Position = u_worldViewProjectionMatrix * a_position;

    // Transform normal to view space.
    mat3 inverseTransposeWorldViewMatrix = mat3(u_inverseTransposeWorldViewMatrix[0].xyz, u_inverseTransposeWorldViewMatrix[1].xyz, u_inverseTransposeWorldViewMatrix[2].xyz);
    v_normalVector = inverseTransposeWorldViewMatrix * a_normal;

	// Transform the camera to view space
	vec4 vert_pos = u_worldMatrix * a_position;
	v_eyeVector = vert_pos.xyz - u_cameraPosition; // eye position in world space
}