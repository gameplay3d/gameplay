// uniforms
cbuffer buffer0 : register(b0)
{
  float4x4 uProjectionMatrix;
};

// inputs
struct VERT_INPUT
{
    float2 Position : POSITION;
    float2 TexCoord  : TEXCOORD0;
    float4 Color : COLOR0;
};

// outputs (frag)
struct FRAG_INPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
    float4 Color    : COLOR;
};


// entry point
FRAG_INPUT main(VERT_INPUT vert)
{
    FRAG_INPUT frag;
    frag.Position = mul(uProjectionMatrix, float4(vert.Position.xy, 0, 1));
    frag.TexCoord = vert.TexCoord;
    frag.Color = vert.Color;
    return frag;
}
