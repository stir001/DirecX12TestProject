#include "stdafx.h"
#include "SkeletonRootSignature.h"
#include "Shader/ShaderCompiler.h"

SkeletonRootSignature::SkeletonRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mShader = ShaderCompiler::Instance().CompileShader(SHADERDIR_PATH + "SkeletonShader.hlsl",
		"SklVS",
		"SklPS",
		"",
		"",
		"",
		true);
	CreateRootSignature("SkeletonRootSignature", mShader.rootSignature.Get(), dev);
}


SkeletonRootSignature::~SkeletonRootSignature()
{
}