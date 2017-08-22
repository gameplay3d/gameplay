struct VSOutput 
{
    float4 Position : SV_POSITION;
    float3 Color    : COLOR;
};

VSOutput main(float4 Position : POSITION, float3 Color : COLOR)
{
    VSOutput result;
    result.Position = Position;
    result.Color = Color;
    return result;
}
