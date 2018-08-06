#include "PrimitiveNormalMapRootSignature.h"
#include "ShaderCompiler.h"

PrimitiveNormalMapRootSignature::PrimitiveNormalMapRootSignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mShader = ShaderCompiler::Instance().CompileShader("DirectX12/Shader/NormalmapShader.hlsl", "NormalMapVS", "NormalMapPS", "NormalMapGS", "", "", true);
	CreateRootSignature(mShader.rootSignature.Get(), dev);
}

PrimitiveNormalMapRootSignature::~PrimitiveNormalMapRootSignature()
{
}
