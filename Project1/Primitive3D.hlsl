#define PRM3DRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
    ", DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL)" \
	", DescriptorTable(CBV(b0), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b1), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(SRV(t1), visibility = SHADER_VISIBILITY_PIXEL)" \
	", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT"   \
        ", addressU = TEXTURE_ADDRESS_WRAP, addressV = TEXTURE_ADDRESS_WRAP, addressW = TEXTURE_ADDRESS_WRAP)"

SamplerState smp : register(s0);

#include "CameraLightcBuffer.hlsl"

struct PriOutput
{
    float4 svpos : SV_POSITION;
    float4 pos : POSITION0;
    float4 shadowpos : POSITION1;
    float3 normal : NORMAL;
    float3 color : COLOR;
    float2 uv : TEXCOORD;
};

[RootSignature(PRM3DRS)]
PriOutput PrimitiveVS(float4 pos : POSITION, float3 normal : NORMAL, float3 color : COLOR, float2 uv : TEXCOORD)
{
    PriOutput po;
    po.svpos = mul(mul(c_viewproj, c_world), pos);
    po.pos = po.svpos;
    po.shadowpos = mul(mul(viewProj, c_world), pos);
    po.color = color;
    po.uv = (float2(1, 1) + po.shadowpos.xy * float2(1, -1)) * 0.5;
    po.normal = normal;
    return po;
}


float4 PrimitivePS(PriOutput data) : SV_Target
{
    return float4(data.color * dot(data.normal, -dir), 1);
}