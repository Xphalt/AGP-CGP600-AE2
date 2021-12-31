cbuffer CBuffer0
{
	// 64 bytes
	matrix WorldViewProjection;
	// 4 bytes
	float red_fraction;
	// 4 bytes
	float scale;
	// 2 x 4 bytes = 8 bytes
	float2 packing;

	// TOTAL SIZE = 80 bytes
};

struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR)
{
	VOut output;

	color.r *= red_fraction;
	output.position = mul(WorldViewProjection, position);
	output.color = color;

	return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
	return color;
}