float4 PS(
    float4 oPosH : SV_POSITION,
    float4 oColor : COLOR) : SV_TARGET
{
    return oColor;
}