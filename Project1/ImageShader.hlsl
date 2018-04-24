#define IMGRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
	", DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL)" \
	", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT"   \
    ", addressU = TEXTURE_ADDRESS_WRAP, addressV = TEXTURE_ADDRESS_WRAP, addressW = TEXTURE_ADDRESS_WRAP)"


Texture2D<float4> tex:register(t0);
SamplerState smp:register(s0);

struct ImageOutput
{
	float4 svpos : SV_POSITION;
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
};

[RootSignature(IMGRS)]
ImageOutput ImageVS(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    ImageOutput o;
	o.pos = pos;
	o.svpos = pos;
	o.uv = uv;
	return o;
}

float4 ImagePS(ImageOutput input) : SV_Target
{
    float4 color = tex.Sample(smp, input.uv);
    if (!color.a) discard;
    return color;
    //return float4(input.uv, 1, 1);
}