precision highp float;

// Uniforms
uniform vec3 u_lightColor;                    // RGB color of the light
uniform vec3 u_ambientColor;                // Ambient color
uniform sampler2D u_diffuseTexture;            // Texture
uniform sampler2D u_normalMapTexture;        // Normalmap

// Inputs
varying vec3 v_lightDirectionTangentSpace;    // light direction
varying vec2 v_texCoord;                    // Texture coordinate.

void main()
{
    // Fetch normals from the normal map
    vec3 normalVector = texture2D(u_normalMapTexture, v_texCoord).rgb * 2.0 - 1.0;
    normalVector = normalize(normalVector);
    
    // Fetch diffuse color from texture
    gl_FragColor = texture2D(u_diffuseTexture, v_texCoord);
    
    // Ambient
    vec3 lightColor = u_ambientColor;

    // Diffuse
    vec3 lightDirection = normalize(v_lightDirectionTangentSpace);
    float diffuseIntensity = clamp(dot(normalVector, lightDirection), 0.0, 1.0);
    lightColor += (u_lightColor * diffuseIntensity);

    // Light the fragment.
    gl_FragColor.rgb *= lightColor;
}