struct ObjectConstants
{
    float4x4 Transform;
    float Scale;
    uint Size;
    float MinHeight;
    float MaxHeight;
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
    uint vertexID       : SV_VertexID,
    out float4 oPosH    : SV_POSITION,
    out float4 oColor   : COLOR)
{
    
    uint sizePlusOne = gObjConstants.Size + 1;
    
    float scale = gObjConstants.Scale;
    float heightWithScale = iHeight * scale;
    
    uint vertexIdPlusSize = vertexID + sizePlusOne;
    uint moduloResult = vertexIdPlusSize % sizePlusOne;
    
    float devideForY = vertexID / sizePlusOne;
    uint rounded = round(devideForY);
    
    oPosH = float4(
        (moduloResult) * gObjConstants.Scale,
        rounded * gObjConstants.Scale,
        iHeight * scale,
        1.0f);
    
    oPosH = mul(oPosH, gPassContants.gViewProj);
    oPosH = mul(oPosH, gPassContants.gProj);
    
    oColor = float4((iHeight - gObjConstants.MinHeight) / (gObjConstants.MaxHeight - gObjConstants.MinHeight), 1.0f, 1.0f, 1.0f);
}