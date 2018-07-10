#define CAMERA_CBUFFER(bnum) cbuffer camera : register(bnum) \
{	\
float4 eye;	\
float4 target;	\
float4x4 c_world; \
float4x4 c_view;	\
float4x4 c_projection;	\
}	


#define LIGHT_CBUFFER(bnum) cbuffer directionalLight : register(bnum)\
{	\
    float3 dir;	\
    float4x4 viewProj;	\
}	