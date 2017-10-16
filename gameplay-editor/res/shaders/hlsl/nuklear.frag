// uniforms
sampler uSampler0 : register(s0);
Texture2D<float4> uTexture0 : register(t0);

// inputs (frag)
struct FRAG_INPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
    float4 Color    : COLOR;
};


// entry point
float4 main(FRAG_INPUT frag) : SV_Target
{
    return frag.Color * uTexture0.Sample(uSampler0, frag.TexCoord);
}