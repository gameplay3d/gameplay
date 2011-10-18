precision highp float;

// Uniforms
uniform vec3 u_lightDirection;              // Direction of the light
uniform vec3 u_lightColor;                  // RGB color of the light
uniform vec3 u_ambientColor;                // Ambient color
uniform sampler2D u_diffuseTexture;         // Diffuse texture

// Inputs
varying vec3 v_normalVector;                // NormalVector in view space (Normalized).
varying vec2 v_texCoord;                    // Texture coordinate (u, v).

void main()
{
    // Ambient
    vec3 lightColor = u_ambientColor;

    // Diffuse
    float diffuseIntensity = max(dot(v_normalVector, u_lightDirection), 0.0);
    lightColor += (u_lightColor * diffuseIntensity);

    // Light the pixel.
    gl_FragColor = texture2D(u_diffuseTexture, v_texCoord);
    gl_FragColor.rgb *= lightColor;
}
