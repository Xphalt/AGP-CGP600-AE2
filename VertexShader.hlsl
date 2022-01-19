cbuffer CBuffer : register(b0)
{
    float xOffSet;
    float yOffSet;
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

    _input.inPosition.x += xOffSet;
    _input.inPosition.y += yOffSet;

    output.outPosition = float4(_input.inPosition, 1.0f);
    output.outTexCoord = _input.inTexCoord;

    return output;
}