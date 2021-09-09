#include "Common.hlsli"

VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    vOut.pos = mul(float4(vIn.pos, 1.0f), g_World);
    vOut.pos = mul(vOut.pos, g_View);
    vOut.pos = mul(vOut.pos, g_Projection);
    vOut.color = vIn.color;
    return vOut;
}
