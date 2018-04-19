#define PMDRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
	", DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL)" \
	", DescriptorTable(CBV(b0), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b1), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b2), visibility = SHADER_VISIBILITY_ALL)" \
    ", DescriptorTable(CBV(b3), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(SRV(t1), visibility = SHADER_VISIBILITY_PIXEL)" \
	", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT"   \
        ", addressU = TEXTURE_ADDRESS_WRAP, addressV = TEXTURE_ADDRESS_WRAP, addressW = TEXTURE_ADDRESS_WRAP)"

Texture2D<float4> tex:register(t0);
Texture2D<float> shadowmap:register(t1);
SamplerState smp:register(s0);
cbuffer mat:register(b0) {
	float3 diffuse;
	float alpha;
	float specularity;
	float3 specular;
	float3 ambient;
	float3 offsetPos;
}

//�{�[���p�s��(���W�X�^1��)
cbuffer bone : register(b1) {
	matrix bones[256];
}

cbuffer camera : register(b2)
{
	float3 eye;
	float3 target;
	matrix c_world;
	matrix c_viewproj;
}

cbuffer directionalLight : register(b3)
{ 
    float3 dir;
    matrix viewProj;
}

struct Output {
	float4 svpos : SV_POSITION;
	float4 pos : POSITION0;
	float4 origpos :POSITION1;
	float4 shadowpos : POSITION2;
	float4 normal : NORMAL;
	float2 uv :TEXCOORD;
};

struct PriOutput {
	float4 svpos : SV_POSITION;
	float4 pos : POSITION0;
	float4 shadowpos : POSITION1;
	float3 normal : NORMAL;
	float3 color : COLOR;
	float2 uv :TEXCOORD;
};

struct MRTOut
{
    float4 normal : SV_Target0;
    float4 specular : SV_Target1;
    float4 amibent : SV_Target2;
};

//���_�����[���[
[RootSignature(PMDRS)]
Output BasicVS(float4 pos : POSITION ,float4 normal : NORMAL,float2 uv : TEXCOORD,min16uint2 boneno : BONENO,min16uint weight : WEIGHT)
{
	float wgt1 = (float)weight / 100.0;
	float wgt2 = 1.0 - wgt1;
	Output o;
	matrix m = bones[boneno[0]] * wgt1 + bones[boneno[1]] * wgt2;
	o.origpos = mul(m, pos) + float4(offsetPos,0);
    o.pos = mul(c_viewproj, (mul(mul(c_world, m), pos) + mul(c_world, float4(offsetPos, 0))));
	o.svpos = o.pos;
	o.shadowpos = mul(mul(viewProj, c_world), pos);
    matrix n = mul(c_world, m);
    n._m03_m13_m23 = 0;

    o.normal = normalize(mul(n,normal));
	o.uv = uv;
	return o;
}

//�s�N�Z���V�F�[�_�[
//[RootSignature(PMDRS)]
float4 BasicPS(Output data) : SV_Target
{
	float3 light = dir;
	float brightness = dot(data.normal.xyz,-light);
	float3 vray = data.origpos.xyz - eye;
	vray = normalize(vray);
    float spec = saturate(pow(max(0.0f,dot(normalize(reflect(-light, data.normal.xyz)), -vray)), specularity));
    float3 color = diffuse * brightness + (specular * spec) + ambient;

    return float4(color, alpha);
}

//[RootSignature(PMDRS)]
float4 ExitTexPS(Output data):SV_Target
{
    float4 color = tex.Sample(smp, data.uv);
    return color * dot(data.normal.xyz, -dir) + color * float4(ambient, 1);
}


PriOutput PrimitiveVS(float4 pos : POSITION, float3 normal : NORMAL, float3 color : COLOR, float2 uv : TEXCOORD)
{
	PriOutput po;
	po.svpos = mul(mul(c_viewproj, c_world), pos);
	po.pos = po.svpos;
	po.shadowpos = mul(mul(viewProj,c_world),pos);
	po.color = color;
	po.uv = (float2(1, 1) + po.shadowpos.xy * float2(1, -1)) * 0.5;
    po.normal = normal;
	return po;
}


float4 PrimitivePS(PriOutput data) : SV_Target
{
	float2 s_uv = (float2(1, 1) + data.shadowpos.xy * float2(1, -1)) * 0.5;
	//if (data.shadowpos.z > shadowmap.Sample(smp, data.uv))
	//{
	//	data.color *= 0.7;
	//}
    // * dot(data.normal, -lightvec)
    //return float4(dir.x, abs(dir.y), dir.z, 1);
    //return float4(dot(data.normal, -dir),1,1, 1);
    return float4(data.color * dot(data.normal, -dir), 1);
}

