#ifdef OPENGL_ES
precision highp float;
#endif

// Uniforms
uniform sampler2D u_diffuseTexture;     // Diffuse texture
uniform vec4 u_diffuseColor;            // Diffuse color/tint

// Inputs
varying vec2 v_texCoord;                // Texture coordinate (u, v).

void main()
{
    gl_FragColor = texture2D(u_diffuseTexture, v_texCoord) * u_diffuseColor;
}
