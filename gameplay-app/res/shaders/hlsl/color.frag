// inputs (frag)
struct FRAG_INPUT
{
    float4 Position : SV_POSITION;
    float3 Color    : COLOR;
};


// entry point
float4 main(FRAG_INPUT input) : SV_TARGET
{
	return float4(input.Color, 1);
}