cbuffer camera : register(b0)
{
    float3 eye;
    float3 target;
    matrix c_world;
    matrix c_viewproj;
}

cbuffer directionalLight : register(b1)
{
    float3 dir;
    matrix viewProj;
}