#include "Basic2DHeader.hlsli"

cbuffer transformCB : register(b0)
{
    matrix mvp;
}

OutVertex main(InVertex invs)
{
    OutVertex outvs;
    outvs.pos = mul(float4(invs.pos, 1.0f), mvp);
    outvs.col = invs.col;
    
    return outvs;
}