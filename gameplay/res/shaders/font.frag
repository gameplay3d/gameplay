#ifdef OPENGL_ES
precision highp float;
#extension GL_OES_standard_derivatives : enable
#endif

// Uniforms
uniform sampler2D u_texture;

// Varyings
varying vec2 v_texCoord;
varying vec4 v_color;

void main()
{ 
    gl_FragColor = v_color;
    float distance = texture2D(u_texture, v_texCoord).a;
    float smoothing = fwidth(distance);
    float alpha = smoothstep(0.5 - smoothing, 0.5 + smoothing, distance);
    gl_FragColor.a = alpha * v_color.a;
}