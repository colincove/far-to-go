struct ObjectConstants
{
    float4x4 gWorldViewProj;
    uint matIndex;
};
ConstantBuffer<ObjectConstants> gObjConstants : register(b0);

void VS(
    float3 iPosL : POSITION,
    float3 iNorm : NORMAL,
    float4 iColor : COLOR,
    float2 iUV : TEXCOORD0,
    out float4 oPosH : SV_POSITION,
    out float4 oColor : COLOR)
{
    // Transform to homogeneous clip space.
    oPosH = mul(float4(iPosL, 1.0f), gObjConstants.gWorldViewProj);
    // Just pass vertex color into the pixel shader.
    oColor = iColor;
}