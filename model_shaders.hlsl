cbuffer CB0
{
	matrix WVPMatrix;
};

Texture2D texture0;
SamplerState sampler0;

struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
};

VOut ModelVS(float4 position : POSITION, float4 colour : COLOR, float2 texcoord : TEXCOORD, float3 normal : NORMAL)
{
	VOut output;

	float4 default_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	output.position = mul(WVPMatrix, position);
	output.color = colour;
	output.texcoord = texcoord;
	output.normal = normal;

	return output;
}

float4 ModelPS(float4 position : SV_POSITION, float4 colour : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{
	//return (1.0f, 1.0f, 1.0f, 1.0f);
	return texture0.Sample(sampler0, texcoord) * colour;
}