#include "MatrixOperation.hlsli"

Texture2D<float4> normalmap : register(t0);
SamplerState smp : register(s0);

#include "CameraLightcBuffer.hlsl"

CAMERA_CBUFFER(b0);
LIGHT_CBUFFER(b1)

cbuffer material : register(b2)
{
	float4 matColor;
}

cbuffer amatrix : register(b3)
{
    matrix amat;
}

struct NormalMapData
{
	float4 svpos : SV_POSITION;
	float4 tangentLight : LIGHT;
	float2 uv : TEXCOORD;
};

struct NormalMapVSInput
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float4 tangent : TANGENT;
    float4 binormal : BINORMAL;
    float2 uv : TEXCOORD;
};

struct PrimitiveVertexData
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

[maxvertexcount(3)]void NormalMapGS(in triangle PrimitiveVertexData vertices[3],inout TriangleStream<NormalMapVSInput> gsOut)
{
    NormalMapVSInput ret[3];
    uint i = 0;
    for (i = 0; i < 3; ++i)
    {
        ret[i].pos = vertices[i].pos;
        ret[i].normal = vertices[i].normal;
        ret[i].uv = vertices[i].uv;
        ret[i].tangent.w = 1.0;
        ret[i].binormal.w = 1.0;
    }

    float3 deltavXUV01 = float3(vertices[0].pos.x - vertices[1].pos.x, vertices[0].uv - vertices[1].uv);
    float3 deltavXUV03 = float3(vertices[0].pos.x - vertices[2].pos.x, vertices[0].uv - vertices[2].uv);
    float3 delatacrossXUV = cross(deltavXUV01, deltavXUV03);
	
    for (i = 0; i < 3; ++i)
    {
        ret[i].tangent.x = -delatacrossXUV.y / delatacrossXUV.x;
        ret[i].binormal.x = -delatacrossXUV.z / delatacrossXUV.x;
    }

    float3 deltavYUV01 = float3(vertices[0].pos.y - vertices[1].pos.y, vertices[0].uv - vertices[1].uv);
    float3 deltavYUV03 = float3(vertices[0].pos.y - vertices[2].pos.y, vertices[0].uv - vertices[2].uv);
    float3 delatacrossYUV = cross(deltavYUV01, deltavYUV03);

    for (i = 0; i < 3; ++i)
    {
        ret[i].tangent.y = -delatacrossXUV.y / delatacrossXUV.x;
        ret[i].binormal.y = -delatacrossXUV.z / delatacrossXUV.x;
    }

    float3 deltavZUV01 = float3(vertices[0].pos.z - vertices[1].pos.z, vertices[0].uv - vertices[1].uv);
    float3 deltavZUV03 = float3(vertices[0].pos.z - vertices[2].pos.z, vertices[0].uv - vertices[2].uv);
    float3 delatacrossZUV = cross(deltavZUV01, deltavZUV03);

    for (i = 0; i < 3; ++i)
    {
        ret[i].tangent.z = -delatacrossZUV.y / delatacrossZUV.x;
        ret[i].binormal.z = -delatacrossZUV.z / delatacrossZUV.x;
    }

    for (i = 0; i < 3; ++i)
    {
        gsOut.Append(ret[i]);
    }
    gsOut.RestartStrip();
}

NormalMapData NormalMapVS(NormalMapVSInput vsIn)
{
    NormalMapData data;
    matrix wvp = mul(c_world, mul(c_view, c_projection));
    data.svpos = mul(wvp, vsIn.pos);
    wvp._14_24_34 = 0;
    matrix tangentspace;
    tangentspace._11_12_13_14 = float4(mul(wvp, vsIn.normal).xyz, 0);
    tangentspace._21_22_23_24 = float4(mul(wvp, vsIn.tangent).xyz, 0);
    tangentspace._31_32_33_34 = float4(mul(wvp, vsIn.binormal).xyz, 0);
    tangentspace._41_42_43_44 = float4(0, 0, 0, 1);
    tangentspace = inverse(tangentspace);
    data.tangentLight = mul(dir, tangentspace);
    data.uv = vsIn.uv;
    return data;
}

float4 NormalmapPS(NormalMapData psIn) : SV_target
{
    float4 normal = normalmap.Sample(smp, psIn.uv);

    float brightness = dot(psIn.tangentLight.xyz, normal.xyz);

    float4 color = saturate(matColor * brightness + matColor * 0.1);
    return color;
}