#ifdef OPENGL_ES
precision highp float;
#endif

//////////////////////////
// Uniforms
uniform sampler2D u_refractionTexture;
uniform sampler2D u_reflectionTexture;
uniform sampler2D u_normalMap;
uniform float u_time;

//////////////////////////
// Varyings
varying vec4 v_vertexRefractionPosition;
varying vec4 v_vertexReflectionPosition;
varying vec2 v_texCoord;
varying vec3 v_eyePosition;

//////////////////////////

const float distortAmount = 0.05;
const float specularAmount = 2.5;
const float textureRepeat = 2.0;
const vec4 tangent = vec4(1.0, 0.0, 0.0, 0.0);
const vec4 viewNormal = vec4(0.0, 1.0, 0.0, 0.0);
const vec4 bitangent = vec4(0.0, 0.0, 1.0, 0.0);
const vec4 waterColour = vec4(0.36, 0.32, 0.2,1.0);

vec2 fromClipSpace(vec4 position)
{
    return position.xy / position.w / 2.0 + 0.5;
}

void main()
{	
    // Get normal
    vec4 normal = texture2D(u_normalMap, v_texCoord * textureRepeat + u_time);
    normal = normalize(normal * 2.0 - 1.0);
    
    // Distortion offset
    vec4 dudv = normal * distortAmount;

    // Refraction sample
    vec2 textureCoord = fromClipSpace(v_vertexRefractionPosition) + dudv.rg;
    textureCoord = clamp(textureCoord, 0.001, 0.999);
    vec4 refractionColour = texture2D(u_refractionTexture, textureCoord) * waterColour;

    // Calc fog distance
    
    // Version 1 (linear)
    float z = (gl_FragCoord.z / gl_FragCoord.w) / 300.0; //const is max fog distance
    const float fogDensity = 6.0;
    float fogAmount = z * fogDensity;
    fogAmount = clamp(fogAmount, 0.0, 1.0);
    
    // Version 2 (exponential)
    //float z = gl_FragCoord.z / gl_FragCoord.w;
    //const float fogDensity = 0.0005;
    //float fogAmount = exp2(-fogDensity * fogDensity * z * z * 1.442695);
    //fogAmount = clamp(fogAmount, 0.0, 0.7);

    refractionColour = mix(refractionColour, waterColour, fogAmount);

    // Sample reflection
    textureCoord = fromClipSpace(v_vertexReflectionPosition) + dudv.rg;
    textureCoord = clamp(textureCoord, 0.001, 0.999);	
    vec4 reflectionColour = texture2D(u_reflectionTexture, textureCoord);

    // View in tangent space
    vec4 viewDir = normalize(vec4(v_eyePosition, 1.0));
    vec4 viewTanSpace = normalize(vec4(dot(viewDir, tangent), dot(viewDir, bitangent), dot(viewDir, viewNormal), 1.0));	
    vec4 viewReflection = normalize(reflect(-1.0 * viewTanSpace, normal));
    float fresnel = dot(normal, viewReflection);	

    vec3 specular = vec3(clamp(pow(dot(viewTanSpace, normal), 255.0), 0.0, 1.0)) * specularAmount;

    gl_FragColor = mix(reflectionColour, refractionColour, fresnel) + vec4(specular, 1.0);
}