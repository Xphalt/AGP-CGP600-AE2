struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTextCoord : TEXCOORD;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT _input) : SV_TARGET
{
    float3 pixelColor = objTexture.Sample(objSamplerState, _input.inTextCoord);

    return float4(pixelColor, 1.0f);
}