#include "Cube.hlsli"

VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    vOut.posH = mul(float4(vIn.posL, 1.0f), g_World);
    vOut.posH = mul(vOut.posH, g_View);
    vOut.posH = mul(vOut.posH, g_Projection);
    vOut.color = vIn.color;
    return vOut;
}
