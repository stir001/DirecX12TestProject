#include "PrimitiveLine3DRootSignature.h"



PrimitiveLine3DRootSignature::PrimitiveLine3DRootSignature()
{
	mShader = ShaderCompiler::Instance().CompileShader(ShaderCompiler::Instance().GetShaderDirPath() + "Primitive3DLine.hlsl",
		"Line3DVS",
		"Line3DPS",
		"Line3DGS",
		"",
		"",
		true
	);

	mInputElements.resize(3);
	mInputElements[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0 ,D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	mInputElements[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	mInputElements[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	CreateRootSignature("Line3D", mShader->rootSignature.Get(), Dx12Ctrl::Instance().GetDev());
}


PrimitiveLine3DRootSignature::~PrimitiveLine3DRootSignature()
{
}
