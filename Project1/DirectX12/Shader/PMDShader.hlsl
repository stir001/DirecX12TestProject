#define PMDRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
	", DescriptorTable(CBV(b0), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b1), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b2), visibility = SHADER_VISIBILITY_ALL)" \
    ", DescriptorTable(CBV(b3), visibility = SHADER_VISIBILITY_ALL)" \

#include "CameraLightcBuffer.hlsl"

CAMERA_CBUFFER(b0)

LIGHT_CBUFFER(b1)

cbuffer bone : register(b2)
{
    matrix bones[256];
}

cbuffer mat:register(b3) {
	float3 diffuse;
	float alpha;
	float specularity;
	float3 specular;
	float3 ambient;
}

struct Output {
	float4 svpos : SV_POSITION;
	float4 pos : POSITION0;
	float4 origpos :POSITION1;
	float4 shadowpos : POSITION2;
	float4 normal : NORMAL;
	float2 uv :TEXCOORD;
};

//頂点しぇーだー
[RootSignature(PMDRS)]
Output BasicVS(float3 pos : POSITION , float3 normal : NORMAL, float2 uv : TEXCOORD, min16uint2 boneno : BONENO, min16uint weight : WEIGHT)
{
	float wgt1 = (float)weight / 100.0;
	float wgt2 = 1.0 - wgt1;
	Output o;
	matrix m = bones[boneno[0]] * wgt1 + bones[boneno[1]] * wgt2;
    o.origpos = mul(m, float4(pos, 1));
    o.pos = mul(c_projection, mul(c_view, (mul(mul(c_world, m), float4(pos, 1)))));
	o.svpos = o.pos;
    o.shadowpos = mul(mul(c_projection, mul(c_view, c_world)), float4(pos, 1));
    matrix n = mul(c_world, m);
    n._m03_m13_m23 = 0;

    o.normal = normalize(mul(n, float4(normal, 1)));
	o.uv = uv;
	return o;
}

//ピクセルシェーダー
float4 BasicPS(Output data) : SV_Target
{
	float3 light = dir;
	float brightness = dot(data.normal.xyz,-light);
	float3 vray = data.origpos.xyz - eye.xyz;
	vray = normalize(vray);
    float spec = saturate(pow(max(0.0f,dot(normalize(reflect(-light, data.normal.xyz)), -vray)), specularity));
    float3 color = diffuse * brightness + (specular * spec) + ambient;

    return float4(color, alpha);
}




