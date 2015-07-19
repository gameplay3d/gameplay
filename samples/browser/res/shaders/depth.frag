#ifdef OPENGL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#endif

varying float v_depth;

// Ported from http://http.developer.nvidia.com/GPUGems/gpugems_ch12.html
// Packs a float into RGBA texture
vec4 pack (float depth)
{
    const vec4 vPack = vec4(1.0, 256.0, 65536.0, 16777216.0);
    return vPack * depth;
}


void main()
{
    gl_FragColor = pack(v_depth);
}