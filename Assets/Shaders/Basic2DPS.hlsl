#include "Basic2DHeader.hlsli"

float4 main(OutVertex outvs) : SV_TARGET
{
    return outvs.col;
}