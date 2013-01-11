#ifdef OPENGL_ES
precision highp float;
#endif

// Uniforms
uniform sampler2D u_texture;

// Inputs
varying vec2 v_texCoord;

void main()
{
    // Sample the diffuse texture for base color
    float gray = dot(texture2D(u_texture, v_texCoord).rgb, vec3(0.299, 0.587, 0.114));
    
    gl_FragColor = vec4(gray, gray, gray, 1.0);
}