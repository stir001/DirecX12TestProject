#define PRM3DRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
    ", DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL)" \
	", DescriptorTable(CBV(b0), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b1), visibility = SHADER_VISIBILITY_ALL)" \
	", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT"   \
        ", addressU = TEXTURE_ADDRESS_WRAP, addressV = TEXTURE_ADDRESS_WRAP, addressW = TEXTURE_ADDRESS_WRAP)"

#include "MatrixOperation.hlsli"

Texture2D<float4> normalmap : register(t0);
SamplerState smp : register(s0);

#include "CameraLightcBuffer.hlsl"

CAMERA_CBUFFER(b0);

LIGHT_CBUFFER(b1)

struct NormalMapData
{
    float4 svpos : SV_POSITION;
    float4 pos : POSITION0;
    float4 normal : NORMAL;
    matrix aMatrix : MATRIX;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct NormalMapVSInput
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    matrix aMat : INSTANCEMAT;
    float4 instanceOffset : INSTANCEPOS;
    uint instanceID : SV_InstanceID;
};

struct PrimitiveVertexData
{
    float4 svpos : SV_POSITION;
    float4 pos : POSITION;
    float4 tangentLight : LIGHT;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

[RootSignature(PRM3DRS)]
NormalMapData NormalMapVS(NormalMapVSInput vsIn)
{
    NormalMapData data;
    matrix pvw = mul(c_projection, mul(c_view, c_world));
    data.svpos = ((mul(pvw, mul(vsIn.aMat, vsIn.pos)) + mul(pvw, vsIn.instanceOffset)));
    data.pos = vsIn.pos;
    data.color = vsIn.color;
    data.uv = vsIn.uv;
    matrix rotaMat = vsIn.aMat;
    rotaMat._14_24_34 = 0;
    rotaMat._41_42_43 = 0;
    //data.normal = mul(rotaMat, vsIn.normal);
    data.normal = vsIn.normal;
    data.aMatrix = rotaMat;
    return data;
}

#define VERTEX_COUNT 3U

[maxvertexcount(VERTEX_COUNT)]void NormalMapGS(in triangle NormalMapData vertices[VERTEX_COUNT], inout TriangleStream<PrimitiveVertexData> gsOut)
{
    PrimitiveVertexData ret[VERTEX_COUNT];
    uint i = 0;
    for (i = 0; i < VERTEX_COUNT; ++i)
    {
        ret[i].svpos = vertices[i].svpos;
        ret[i].pos = vertices[i].pos;
        ret[i].color = vertices[i].color;
        ret[i].uv = vertices[i].uv;
    }
	
    matrix tangentSpace;
    tangentSpace._14_24_34 = 0.0f;
    tangentSpace._31_32_33 = vertices[0].normal.xyz;
    tangentSpace._41_42_43 = 0.0f;
    tangentSpace._44 = 1.0f;

    float3 deltavXUV01 = float3(ret[0].pos.x - ret[1].pos.x, ret[0].uv - ret[1].uv);
    float3 deltavXUV03 = float3(ret[0].pos.x - ret[2].pos.x, ret[0].uv - ret[2].uv);
    float3 delatacrossXUV = cross(deltavXUV01, deltavXUV03);
	
    tangentSpace._11 = -delatacrossXUV.y / delatacrossXUV.x;
    tangentSpace._21 = -delatacrossXUV.z / delatacrossXUV.x;

    float3 deltavYUV01 = float3(ret[0].pos.y - ret[1].pos.y, ret[0].uv - ret[1].uv);
    float3 deltavYUV03 = float3(ret[0].pos.y - ret[2].pos.y, ret[0].uv - ret[2].uv);
    float3 delatacrossYUV = cross(deltavYUV01, deltavYUV03);

    tangentSpace._12 = -delatacrossYUV.y / delatacrossYUV.x;
    tangentSpace._22 = -delatacrossYUV.z / delatacrossYUV.x;

    float3 deltavZUV01 = float3(ret[0].pos.z - ret[1].pos.z, ret[0].uv - ret[1].uv);
    float3 deltavZUV03 = float3(ret[0].pos.z - ret[2].pos.z, ret[0].uv - ret[2].uv);
    float3 delatacrossZUV = cross(deltavZUV01, deltavZUV03);

    tangentSpace._13 = -delatacrossZUV.y / delatacrossZUV.x;
    tangentSpace._23 = -delatacrossZUV.z / delatacrossZUV.x;

    tangentSpace._11_12_13 = normalize(tangentSpace._11_12_13);
    tangentSpace._21_22_23 = normalize(tangentSpace._21_22_23);
    tangentSpace._31_32_33 = normalize(tangentSpace._31_32_33);

    tangentSpace = transpose(tangentSpace);
    matrix localMat = inverse(vertices[0].aMatrix);
    float4 tangentLight = mul(mul(localMat, dir), tangentSpace);

    for (i = 0; i < VERTEX_COUNT; ++i)
    {
        ret[i].tangentLight = float4(normalize(tangentLight.xyz), 1.0);
        gsOut.Append(ret[i]);
    }
    gsOut.RestartStrip();
}

float4 NormalMapPS(PrimitiveVertexData psIn) : SV_target
{
    float3 normal = normalize(((normalmap.Sample(smp, psIn.uv)) * 2.0f - 1.0f).xyz);

    float brightness = saturate(dot(-psIn.tangentLight.xyz, normalize(normal.xyz)));
    return float4(brightness, brightness, brightness, 1);

    //float4 color = saturate(float4((psIn.color * brightness + psIn.color * 0.1).xyz, 1.0f));
    //return color;
}