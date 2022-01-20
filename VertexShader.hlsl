cbuffer worldPositionBuffer : register(b0)
{
    matrix WVPMatrix;
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

    output.outPosition = mul(WVPMatrix, _input.inPosition);
    output.outTexCoord = _input.inTexCoord;
    return output;
}