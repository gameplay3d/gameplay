precision highp float;

// Uniforms
uniform vec3 u_lightDirection;            // Direction of the light
uniform vec3 u_lightColor;                // RGB color of the light
uniform vec3 u_ambientColor;            // Ambient color
uniform float u_specularExponent;        // Specular exponent or shininess property.
uniform sampler2D u_diffuseTexture;        // Diffuse texture.

// Inputs
varying vec3 v_normalVector;            // NormalVector in view space.
varying vec2 v_texCoord;                // Texture coordinate (u, v).
varying vec3 v_cameraDirection;         // Camera direction

void main()
{
    // Fetch diffuse color from texture.
    vec4 diffuseColor = texture2D(u_diffuseTexture, v_texCoord);
    
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

    // Light the pixel
    gl_FragColor = diffuseColor;
    gl_FragColor.rgb *= lightColor;
    gl_FragColor.rgb += vec3(diffuseColor.rgb * specularIntensity);
}
