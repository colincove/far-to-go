struct ObjectConstants
{
    float4x4 gWorldViewProj;
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

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
};

struct VertexIn
{
    float3 PosL : POSITION;
    float3 Norm : NORMAL;
    float4 Color : COLOR;
    float2 UV : TEXCOORD0;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
// Transform to homogeneous clip space.
    float4 posW = mul(float4(vin.PosL, 1.0f), gObjConstants.gWorldViewProj);
    vout.PosH = mul(posW, gPassContants.gViewProj);
// Just pass vertex color into the pixel shader.
    vout.Color = vin.Color;
    return vout;
}