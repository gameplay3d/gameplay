precision mediump float;

// Diffuse color
uniform vec4 u_diffuseColor;        // Diffuse color

void main()
{
    gl_FragColor = u_diffuseColor;
}