#ifdef OPENGL_ES
precision highp float;
#endif

// Uniforms
uniform samplerCube u_cubeTexture;    // Enviroment map

// Inputs
varying vec3 v_normalVector;          // Normal vector in view space
varying vec3 v_eyeVector;             // Eye vector in world space

void main()
{
    vec3 N = normalize(v_normalVector);
    vec3 E = normalize(v_eyeVector);
    vec3 R = reflect(E, N);

    gl_FragColor = textureCube(u_cubeTexture, R);
}