#include "Common.hlsli"

//VertexPosHWNormalTex VS(VertexPosNormalTex vIn)
//{
//    VertexPosHWNormalTex vOut;
//    vOut.PosH = mul(float4(vIn.PosL, 1.0f), g_World);
//    vOut.PosH = mul(vOut.PosH, g_View);
//    vOut.PosH = mul(vOut.PosH, g_Projection);
//    vOut.color = vIn.color;
//    return vOut;
//}

VertexPosHWNormalTex VS(VertexPosNormalTex vIn)
{
    VertexPosHWNormalTex vOut;
    float4 PositionW = mul(float4(vIn.PosL, 1.0f), g_World);
    vOut.PosH = mul(PositionW, g_View);
    vOut.PosH = mul(vOut.PosH, g_Projection);
    vOut.PosW = PositionW.xyz;
    //vOut.NormalW = mul(vIn.NormalL, (float3x3) g_WorldInvTranspose);
    //vOut.Tex = vIn.Tex;
    vOut.color = vIn.color;
    return vOut;
}

VertexOut VS_reference(VertexIn vIn)
{
    VertexOut vOut;
    vOut.pos = mul(float4(vIn.pos, 1.0f), g_World);
    vOut.pos = mul(vOut.pos, g_View);
    vOut.pos = mul(vOut.pos, g_Projection);
    vOut.color = vIn.color;
    return vOut;
}

//VertexPosHWNormalTex VS_back(VertexPosNormalTex vIn)
//{
//    VertexPosHWNormalTex vOut;
//    matrix viewProj = mul(g_View, g_Projection);
//    float4 posW = mul(float4(vIn.PosL, 1.0f), g_World);
//
//    vOut.PosH = mul(posW, viewProj);
//    vOut.PosW = posW.xyz;
//    vOut.NormalW = mul(vIn.NormalL, (float3x3) g_WorldInvTranspose);
//    vOut.Tex = vIn.Tex;
//    return vOut;
//}
