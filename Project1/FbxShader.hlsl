#define FBXRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
    ", DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL)" \
	", DescriptorTable(CBV(b0), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b1), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b2), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(SRV(t1), visibility = SHADER_VISIBILITY_PIXEL)" \
    ", DescriptorTable(SRV(t2), visibility = SHADER_VISIBILITY_PIXEL)" \
	", DescriptorTable(SRV(t3), visibility = SHADER_VISIBILITY_PIXEL)" \
	", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT"   \
        ", addressU = TEXTURE_ADDRESS_WRAP, addressV = TEXTURE_ADDRESS_WRAP, addressW = TEXTURE_ADDRESS_WRAP)"

Texture2D<float4> diffsemap:register(t0);
Texture2D<float> ambientmap:register(t1);
Texture2D<float4> specularmap:register(t2);
Texture2D<float4> normalmap:register(t3);
SamplerState smp : register(s0);

#include "CameraLightcBuffer.hlsl"

cbuffer offset: register(b2)
{
    float3 offsetpos;
}

struct Input
{
    float3 pos :POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct Output
{
    float4 svpos : SV_POSITION;
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
};

[RootSignature(FBXRS)]
Output FbxVS(Input input)
{
    Output output;
    output.svpos = mul(c_viewproj, (mul(c_world, float4(input.pos, 1)) /*+ mul(c_world, float4(offsetpos, 1))*/));
    output.pos = output.svpos;
    output.uv = input.uv;

    matrix m = c_world;
    m._m03_m13_m23 = 0;
    float3 n = normalize(input.normal);
    output.normal = (mul(m, float4(n, 1)));

    return output;
}

//�e�N�X�`����B8G8R8A8�̎��̎���?
float4 FbxPS(Output output) : SV_Target
{
    //return float4(output.uv, 0, 1);
    //return abs(float4(output.normal));
    //return normalmap.Sample(smp, output.uv);

    //float4 mapNormal = float4(normalize(normalmap.Sample(smp, output.uv).xyz), 1);
    //float c = mul(output.normal.xyz, mapNormal.xyz);
    //float rad = acos(c) / 2.0f;
    //float3 axis = normalize(cross(mapNormal.xyz, output.normal.xyz));
    //float4 q = { axis.x * sin(rad), axis.y * sin(rad), axis.z * sin(rad), cos(rad) };
    //float4 nq = { -axis.x * sin(rad), -axis.y * sin(rad), -axis.z * sin(rad), cos(rad) };
    //float4 mulNormal = mul(mul(nq, mapNormal), q);

    //float3 n_munorm = normalize(mulNormal.xyz);

    ////return abs(mulNormal);
    //return abs(float4((n_munorm.x),0,0, 1));

    float4 light = float4(dir, 1);
    float brightness = dot(output.normal.xyz , light.xyz);
    float4 color = diffsemap.Sample(smp, output.uv);
    color = color /** brightness*/ /*+ color * ambientmap.Sample(smp, output.uv)*/;

    return color;
}