cbuffer CBuffer : register(b0)
{
    float4x4 matrix_;
};

struct VS_INPUT
{
    float3 inPosition : POSITION;
    float2 inTexCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 outPosition : SV_POSITION;
    float2 outTexCoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT _input)
{
    VS_OUTPUT output;

    output.outPosition = mul(float4(_input.inPosition, 1.0f), matrix_);
    output.outTexCoord = _input.inTexCoord;

    return output;
}