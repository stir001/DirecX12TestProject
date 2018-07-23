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

//cbuffer mcolor : register(b3)
//{
//    float4 modelColor;
//}

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
	//output.svpos = mul(c_viewproj, (mul(c_world, float4(input.pos, 1))));
	//o.svpos = mul(mul(mul(float4(input.pos, 1), modelMatrix), c_world), c_viewproj);
    o.pos = o.svpos;
    o.uv = input.uv;


    matrix m = mul(c_world, modelMatrix);
    m._m03_m13_m23 = 0;
    o.normal = float4(normalize(mul(m,input.normal).xyz),0.0f);

    return o;
}

float4 FbxPS(Output output) : SV_Target
{
  //return abs(output.normal);
  //return float4(output.uv, 0, 1);
	float bright = dot(output.normal.xyz, -dir);
    float4 diffusecolor = (diffsemap.Sample(smp, output.uv));
   return saturate(float4(saturate(diffusecolor.rgb * bright) + 0.5f * diffusecolor.rgb, 1));
  //return saturate(float4(saturate(modelColor.rgb * bright) + 0.5f * modelColor.rgb, 1));
  //return abs(float4(output.normal));
  //return float4(normalize(normalmap.Sample(smp, output.uv).xyz), 1);
 
  //float4 mapNormal = float4(normalmap.Sample(smp, output.uv).xyz, 1);
  //mapNormal.xyz -= 0.5f;
  //mapNormal.xyz *= 2.0f;
  //mapNormal.xyz = normalize(mapNormal.xyz);
  //float c = mul(output.normal.xyz, mapNormal.xyz);
  //float rad = acos(c) / 2.0f;
  //float3 axis = normalize(cross(mapNormal.xyz, output.normal.xyz));
  //float4 q = { axis.x * sin(rad), axis.y * sin(rad), axis.z * sin(rad), cos(rad) };
  //float4 nq = { -axis.x * sin(rad), -axis.y * sin(rad), -axis.z * sin(rad), cos(rad) };
  //float4 mulNormal = mul(mul(nq, mapNormal), q);
 
  //float3 n_munorm = normalize(mulNormal.xyz);
 
  ////return abs(mulNormal);
  //return float4(abs(n_munorm.xyz), 1);
 
  //float4 light = float4(dir, 1);
  //float brightness = dot(n_munorm.xyz, light.xyz);
  //float4 color = diffsemap.Sample(smp, output.uv);
  //color = color * brightness + color * ambientmap.Sample(smp, output.uv);
 
  //return ambientmap.Sample(smp, output.uv);
  //return color;
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
