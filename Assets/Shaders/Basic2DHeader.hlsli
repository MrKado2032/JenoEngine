struct InVertex
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct OutVertex
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};