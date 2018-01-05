// inputs
struct VERT_INPUT
{
    float4 Position : POSITION;
    float3 Color : COLOR;
};


// outputs (frag)
struct FRAG_INPUT
{
    float4 Position : SV_POSITION;
    float3 Color    : COLOR;
};


// entry point
FRAG_INPUT main(VERT_INPUT vert)
{
    FRAG_INPUT frag;
    frag.Position = vert.Position;
    frag.Color = vert.Color;
    return frag;
}