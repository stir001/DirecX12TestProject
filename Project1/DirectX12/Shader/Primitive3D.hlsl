#define PRM3DRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
    ", DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL)" \
	", DescriptorTable(CBV(b0), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b1), visibility = SHADER_VISIBILITY_ALL)" \
	", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT"   \
        ", addressU = TEXTURE_ADDRESS_WRAP, addressV = TEXTURE_ADDRESS_WRAP, addressW = TEXTURE_ADDRESS_WRAP)"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

#include "CameraLightcBuffer.hlsl"

CAMERA_CBUFFER(b0)

LIGHT_CBUFFER(b1)

struct PriOutput
{
    float4 svpos : SV_POSITION;
    float4 pos : POSITION0;
    float4 normal : NORMAL;
    float4 color : COLOR;
    //float2 uv : TEXCOORD;
};

[RootSignature(PRM3DRS)]
PriOutput PrimitiveVS(float4 pos : POSITION, float4 normal : NORMAL, float4 color : COLOR, float2 uv : TEXCOORD)
{
    PriOutput po;
    po.svpos = mul(mul(c_projection, mul(c_view, c_world)), pos);
    po.pos = po.svpos;
    po.color = color;
   // po.uv = (float2(1, 1) + po.shadowpos.xy * float2(1, -1)) * 0.5;
    po.normal = normal;
    return po;
}

float4 PrimitivePS(PriOutput data) : SV_Target
{
    return float4(data.color * dot(data.normal, float4(-dir, 1)));
}