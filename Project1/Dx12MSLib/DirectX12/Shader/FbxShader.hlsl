#include "FbxRootsiganatureDefine.hlsli"

#define SMAPLWEDEFINE ", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT"   \
        ", addressU = TEXTURE_ADDRESS_WRAP, addressV = TEXTURE_ADDRESS_WRAP, addressW = TEXTURE_ADDRESS_WRAP)"

Texture2D<float4> diffsemap:register(t0);
Texture2D<float4> specularmap:register(t1);
Texture2D<float4> bumpmap:register(t2);
SamplerState smp : register(s0);

#include "CameraLightcBuffer.hlsl"

CAMERA_CBUFFER(b0)

LIGHT_CBUFFER(b1)

cbuffer offset: register(b2)
{
   float4x4 modelMatrix;
}

struct Input
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    //float4x4 vmatrix : VERTEXMATRIX;
};

struct Output
{
    float4 svpos : SV_POSITION;
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct PSOutput
{
    float2 normal   : Sv_Target0;
    float4 specular : Sv_Target1;
    float4 albedo   : Sv_Target2;
};

[RootSignature(FBXRS SMAPLWEDEFINE)]
Output FbxVS(Input input)
{
    Output o;
    o.svpos = mul(c_projection, mul(c_view, ( mul(c_world, mul(modelMatrix, input.pos)))));
    o.pos = o.svpos;
    o.uv = input.uv;

    matrix m = mul(c_world, modelMatrix);
    m._m03_m13_m23 = 0;
    o.normal = float4(normalize(mul(m,input.normal).xyz),0.0f);

    return o;
}

float4 FbxPS(Output output) : SV_Target
{
	float bright = dot(output.normal.xyz, -dir.xyz);
    float4 diffusecolor = (diffsemap.Sample(smp, output.uv));
   return saturate(float4(saturate(diffusecolor.rgb * bright) + 0.5f * diffusecolor.rgb, 1));
}

float2 PackingNormal(float2 viewNorm)
{
    return float2(0.5 * (viewNorm.xy + 1.0f));
}

PSOutput FBXGeometryPS(Output input)
{
    PSOutput output;
    float2 viewNorm = mul(c_view,input.normal).xy;
    viewNorm = PackingNormal(viewNorm);//法線パッキング
    output.normal = viewNorm;
    output.albedo = (diffsemap.Sample(smp, input.uv.xy));
    output.specular = specularmap.Sample(smp, input.uv.xy);

    return output;
}
