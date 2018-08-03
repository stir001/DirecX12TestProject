#include "PrimitiveRootSignature.h"
#include "ShaderCompiler.h"


PrimitiveRootSignature::PrimitiveRootSignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mShader = ShaderCompiler::Instance().CompileShader("DirectX12/Shader/Primitive3D.hlsl",
		"PrimitiveVS",
		"PrimitivePS", "", "", "", true);
	CreateRootSignature(mShader.rootSignature.Get(), dev);
}

PrimitiveRootSignature::~PrimitiveRootSignature()
{
}
