#include "Basic2DHeader.hlsli"

cbuffer transformCB : register(b0)
{
    matrix mvp;
}

cbuffer colorCB : register(b1)
{
    float4 color;
}

OutVertex main(InVertex invs)
{
    OutVertex outvs;
    outvs.pos = mul(float4(invs.pos, 1.0f), mvp);
    outvs.col = color;
    
    return outvs;
}