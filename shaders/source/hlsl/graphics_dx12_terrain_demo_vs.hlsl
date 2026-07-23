struct ObjectConstants
{
    float4x4 World;
    float Scale;
    uint Size;
    float MinHeight;
    float MaxHeight;
    float4x4 WorldViewProj;
    float4x4 WorldViewProjInverseTranspose;
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
    float iHeight       : HEIGHT,
    float4 iNormal      : NORMAL,
    uint vertexID       : SV_VertexID,
    out float4 oPosH    : SV_POSITION,
    out float4 oNormal  : NORMAL,
    out float4 oColor   : COLOR)
{
    
    uint sizePlusOne = gObjConstants.Size + 1;
    
    float scale = gObjConstants.Scale;
    
    uint vertexIdPlusSize = vertexID + sizePlusOne;
    uint moduloResult = vertexIdPlusSize % sizePlusOne;
    
    float devideForY = vertexID / sizePlusOne;
    uint rounded = round(devideForY);
    
    oPosH = float4(
        (moduloResult) * gObjConstants.Scale,
        iHeight * scale,
        rounded * gObjConstants.Scale,
        1.0f);
    
    oPosH = mul(oPosH, gObjConstants.World);
    oPosH = mul(oPosH, gPassContants.gView); 
    oPosH = mul(oPosH, gPassContants.gProj);
    
    //oPosH = mul(oPosH, gPassContants.gView);
    float HeightRange = gObjConstants.MaxHeight - gObjConstants.MinHeight;
    float HeightValueFromMin = iHeight - gObjConstants.MinHeight;
    float HeightFraction = HeightValueFromMin / HeightRange;
    oColor = float4(HeightFraction, HeightFraction / 2, HeightFraction / 2, 1.0f);
}