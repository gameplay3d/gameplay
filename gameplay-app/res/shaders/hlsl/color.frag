struct VSOutput 
{
    float4 Position : SV_POSITION;
    float3 Color    : COLOR;
};

Texture2D uTex0 : register(t1);
SamplerState uSampler0 : register(s2);

float4 main(VSOutput input) : SV_Target
{
    return float4(input.Color, 1);
}