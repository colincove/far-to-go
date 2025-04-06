cbuffer cbPerObject : register(b0)
{
    float4x4 gWorldViewProj;
};

void VS(
    float3 iPosL : POSITION,
    float4 iColor : COLOR,
    out float4 oPosH : SV_POSITION,
    out float4 oColor : COLOR)
{
    // Transform to homogeneous clip space.
    oPosH = mul(float4(iPosL, 1.0f), gWorldViewProj);
    // Just pass vertex color into the pixel shader.
    oColor = iColor;
}

float4 PS(
    float4 oPosH,
    float4 oColor)
{
    return oColor;
}