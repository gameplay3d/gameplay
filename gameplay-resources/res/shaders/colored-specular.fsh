precision highp float;

// Uniforms
uniform    vec3 u_lightDirection;        // Direction of the light
uniform vec3 u_lightColor;            // RGB color of the light
uniform vec3 u_ambientColor;        // Ambient color
uniform vec4 u_diffuseColor;        // Diffuse color
uniform float u_specularExponent;    // Specular exponent or shininess property.

// Inputs
varying vec3 v_normalVector;        // Normal vector in view space.
varying vec3 v_cameraDirection;        // Camera direction.

void main()
{
    // Normalize the vectors.
    vec3 cameraDirection = normalize(v_cameraDirection);
    vec3 lightDirection = normalize(u_lightDirection);
    vec3 normalVector = normalize(v_normalVector);
    
    // Ambient
    vec3 lightColor = u_ambientColor;

    // Diffuse
    float diffuseIntensity = max(0.0, dot(normalVector, lightDirection));
    lightColor += (u_lightColor * diffuseIntensity);

    // Specular
    vec3 halfVector = normalize(cameraDirection + lightDirection);
    float specularIntensity = pow(max(0.0, dot(normalVector, halfVector)), u_specularExponent);

    // Light the pixel.
    gl_FragColor = u_diffuseColor;
    gl_FragColor.rgb *= lightColor;
    gl_FragColor.rgb += (u_diffuseColor.rgb * specularIntensity);
}