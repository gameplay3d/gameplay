precision highp float;

// Uniforms
uniform vec3 u_lightColor;                        // RGB color of the light
uniform vec3 u_ambientColor;                    // Ambient color
uniform float u_specularExponent;                // Specular exponent or shininess property.
uniform sampler2D u_diffuseTexture;                // Diffuse texture
uniform sampler2D u_normalMapTexture;            // Normal map texture

// Inputs
varying vec2 v_texCoord;                        // Texture Coordinate (u,v)
varying vec3 v_lightDirectionTangentSpace;        // Direction of the light in tangent space
varying vec3 v_cameraDirectionTangentSpace;     // Direction the camera is looking at in tangent space

void main()
{
    // Fetch diffuse color from texture
    vec4 diffuseColor = texture2D(u_diffuseTexture, v_texCoord);
    
    // Normalize vectors
    vec3 cameraDirection = normalize(v_cameraDirectionTangentSpace);
    vec3 lightDirection = normalize(v_lightDirectionTangentSpace);
    // Fetch normals from the normal map
    vec3 normalVector = normalize(texture2D(u_normalMapTexture, v_texCoord).rgb * 2.0 - 1.0);
    
    // Ambient
    vec3 lightColor = u_ambientColor;

    // Diffuse
    float diffuseIntensity = max(0.0, dot(normalVector, lightDirection));
    lightColor += (u_lightColor * diffuseIntensity);

    // Specular
    vec3 halfVector = normalize(cameraDirection + lightDirection);
    float specularIntensity = pow(max(0.0, dot(normalVector, halfVector)), u_specularExponent);
    
    // Light the pixel.
    gl_FragColor = diffuseColor;
    gl_FragColor.rgb *= lightColor;
    gl_FragColor.rgb += vec3(diffuseColor.rgb * specularIntensity);
}
