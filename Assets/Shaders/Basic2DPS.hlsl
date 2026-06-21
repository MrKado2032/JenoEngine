#include "Basic2DHeader.hlsli"

Texture2D g_tex2D : register(t0);
SamplerState g_sampler : register(s0);

cbuffer colorCB : register(b1)
{
    float4 color;
}

float4 main(OutVertex outvs) : SV_TARGET
{
    return g_tex2D.Sample(g_sampler, outvs.uv) * color;
}