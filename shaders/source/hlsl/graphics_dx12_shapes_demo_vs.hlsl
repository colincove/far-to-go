struct ObjectConstants
{
    float4x4 gWorldViewProj;
    uint matIndex;
};

ConstantBuffer<ObjectConstants> gObjConstants : register(b0);

struct PassConstants
{
    float4x4 gView;
    float4x4 gInvView;
    float4x4 gProj;
    float4x4 gInvProj;
    float4x4 gViewProj;
    float4x4 gInvViewProj;
    float3 gEyePosW;
    float cbPerObjectPad1;
    float2 gRenderTargetSize;
    float2 gInvRenderTargetSize;
    float gNearZ;
    float gFarZ;
    float gTotalTime;
    float gDeltaTime;
};

ConstantBuffer<PassConstants> gPassContants : register(b1);

void VS(
    float3 iPosL        : POSITION,
    float4 iNorm        : NORMAL,
    float3 iTan         : TANGENT,
    float4 iColor       : COLOR,
    float2 iUV          : TEXCOORD0,
    out float4 oPosH    : SV_POSITION,
    out float4 oColor   : COLOR)
{
    // Transform to homogeneous clip space.
    oPosH = mul(float4(iPosL, 1.0f), gObjConstants.gWorldViewProj);
    oPosH = mul(float4(iPosL, 1.0f), gPassContants.gView);
    // Just pass vertex color into the pixel shader.
    oColor = iColor;
}