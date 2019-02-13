#include "stdafx.h"
#include "PMDToonRootsignature.h"
#include "Shader/ShaderCompiler.h"


PMDToonRootsignature::PMDToonRootsignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	ShaderCompiler::Instance().AddDefineMacro("CAMERA_REGISTER", "b0");
	ShaderCompiler::Instance().AddDefineMacro("LIGHT_REGISTER", "b1");

	mShader = ShaderCompiler::Instance().CompileShader(ShaderCompiler::Instance().GetShaderDirPath() + "PMDToonShader.hlsl"
		, "PmdToonVS"
		, "PmdToonPS"
		, "PmdGS"
		, ""
		, ""
		, true);
	CreateRootSignature("PmdToon", mShader.rootSignature.Get(), dev);
}


PMDToonRootsignature::~PMDToonRootsignature()
{
}
