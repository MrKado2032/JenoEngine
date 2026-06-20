struct InVertex
{
    float3 pos : POSITION;
    float4 col : COLOR;
};

struct OutVertex
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
};