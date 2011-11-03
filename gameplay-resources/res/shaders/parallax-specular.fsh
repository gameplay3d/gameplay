precision highp float;

// Uniforms
uniform vec3 u_lightColor;                      // RGB color of the light
uniform vec3 u_ambientColor;                    // Ambient color
uniform float u_specularExponent;               // Specular exponent or shininess property.
uniform float u_parallaxHeight;                 // Parallax height
uniform sampler2D u_diffuseTexture;             // Diffuse texture
uniform sampler2D u_bumpMapTexture;             // Height map texture
uniform sampler2D u_normalMapTexture;           // Normal map texture    

// Inputs
varying vec2 v_texCoord;                        // Texture coordinates (u,v)
varying vec3 v_lightDirectionTangentSpace;      // Light direction in tangent space
varying vec3 v_cameraDirectionTangentSpace;     // Camera direction in tangent space

void main()
{
    // Normalize vectors.
    vec3 lightDirection = normalize(v_lightDirectionTangentSpace);
    vec3 cameraDirection = normalize(v_cameraDirectionTangentSpace);
    
    // Get the height in the bumpmap texture.
    float height = texture2D(u_bumpMapTexture, v_texCoord).r;
    
    // Compute offset within the range (-parallax height to +parallax height).
    float offset = u_parallaxHeight * (2.0 * height - 1.0);
    vec2 parallaxTexCoord = v_texCoord + (cameraDirection.xy * offset);

    // Compute offset again with the new texture coordinates to get better precision.
    // Find out the mean height.
    height += texture2D(u_bumpMapTexture, parallaxTexCoord).r;
    offset = u_parallaxHeight * (height - 1.0);
    parallaxTexCoord = v_texCoord + (cameraDirection.xy * offset);
    
    // Fetch diffuse color from texture.
    vec4 diffuseColor = texture2D(u_diffuseTexture, parallaxTexCoord);
    
    // Ambient
    vec3 lightColor = u_ambientColor;

    // Diffuse
    // Fetch normals from the normal map with the modified texture coordinates.
    vec3 normalVector = normalize(texture2D(u_normalMapTexture, parallaxTexCoord).xyz * 2.0 - 1.0);
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