// Inputs
attribute vec4 a_position;                          // Vertex Position (x, y, z, w)
attribute vec3 a_normal;                            // Vertex Normal (x, y, z)

// Uniforms
uniform mat4 u_worldViewProjectionMatrix;           // Matrix to transform a position to clip space.
uniform mat4 u_inverseTransposeWorldMatrix;         // Matrix to transform a normal to view space.

// Outputs
varying vec3 v_normalVector;                        // Normal vector in view space.

void main()
{
	// Vertex attributes
    vec4 position = a_position;
    vec3 normal = a_normal;
        
    // Transform position to clip space.
    gl_Position = u_worldViewProjectionMatrix * position;

    // Transform normal to view space.
    mat3 inverseTransposeWorldViewMatrix = mat3(u_inverseTransposeWorldMatrix[0].xyz, u_inverseTransposeWorldMatrix[1].xyz, u_inverseTransposeWorldMatrix[2].xyz);
    v_normalVector = inverseTransposeWorldViewMatrix * normal;
}