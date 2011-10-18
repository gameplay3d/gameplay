precision highp float;

// Uniforms
uniform vec3 u_lightDirection;        // Direction of the light
uniform vec3 u_lightColor;            // RGB color of the light
uniform vec3 u_ambientColor;        // Ambient color
uniform vec4 u_diffuseColor;        // Diffuse color

// Inputs
varying vec3 v_normalVector;        // NormalVector in view space (Normalized).

void main()
{
    // Ambient
    vec3 lightColor = u_ambientColor;

    // Diffuse
    float diffuseIntensity = max(dot(v_normalVector, u_lightDirection), 0.0);
    lightColor += (u_lightColor * diffuseIntensity);

    // Light the pixel.
    gl_FragColor = u_diffuseColor;
    gl_FragColor.rgb *= lightColor;
}