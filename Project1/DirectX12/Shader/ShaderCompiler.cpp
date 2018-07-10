#include "ShaderCompiler.h"
#include "Util.h"
#include "CharToWChar.h"
#include "HlslInclude.h"

#include <d3d12.h>
#include <d3dcompiler.h>
#include <functional>
#include <d3dcommon.h>

using Microsoft::WRL::ComPtr;

ShaderCompiler* ShaderCompiler::mInstance = nullptr;

ShaderCompiler::ShaderCompiler()
{
}

ShaderCompiler::~ShaderCompiler()
{
	for (auto& data : mDatas)
	{
		data.second.vertexShader.Reset();
		data.second.pixelShader.Reset();
		data.second.geometryShader.Reset();
		data.second.domainShader.Reset();
		data.second.hullShader.Reset();
		data.second.rootSignature.Reset();
	}
}

ShaderDatas ShaderCompiler::CompileShader(const std::string& shaderPath,
	const std::string& vsName,
	const std::string& psName,
	const std::string& gsName,
	const std::string& hsName,
	const std::string& dsName, bool existRootSignature)
{

	auto itr = mDatas.find(shaderPath);
	if (itr != mDatas.end())
	{
		return itr->second;
	}
#ifdef _DEBUG
	UINT compileflag = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	std::function<void(ID3D10Blob*)> outErr = [](ID3D10Blob* err) {
		if (err != nullptr)
		{
			OutputDebugStringA((char*)err->GetBufferPointer());
		}
	};
#else
	std::function<void(ID3D10Blob*)> outErr = [](ID3D10Blob* err) {
	};
	UINT compileflag = D3DCOMPILE_ENABLE_STRICTNESS;
#endif

	HlslInclude hlslinculde;
	ID3D10Blob* err = nullptr;

	std::string rpath = GetRelativePath(shaderPath);
	hlslinculde.SetRelativePath(rpath);

	wchar_t* path = nullptr;
	ToWChar(&path, shaderPath);

	HRESULT result;

	if (vsName.size() > 0)
	{
		ID3DBlob* vertex = nullptr;
		result = D3DCompileFromFile(path, nullptr, &hlslinculde,
			vsName.data(), "vs_5_0", compileflag, 0, &vertex, &err);
		outErr(err);
		mDatas[shaderPath].vertexShader.Swap(vertex);

		if (existRootSignature)
		{
			ID3DBlob* root = nullptr;
			result = D3DGetBlobPart(vertex->GetBufferPointer() , vertex->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, &root);
			mDatas[shaderPath].rootSignature.Swap(root);
		}
	}

	if (psName.size() > 0)
	{
		ID3DBlob* pixcel = nullptr;
		result = D3DCompileFromFile(path, nullptr, &hlslinculde,
			psName.data(), "ps_5_0", compileflag, 0, &pixcel, &err);
		outErr(err);
		mDatas[shaderPath].pixelShader.Swap(pixcel);
	}

	if (gsName.size() > 0)
	{
		ID3DBlob* geometry = nullptr;
		result = D3DCompileFromFile(path, nullptr, &hlslinculde,
			gsName.data(), "gs_5_0", compileflag, 0, &geometry, &err);
		outErr(err);
		mDatas[shaderPath].geometryShader.Swap(geometry);
	}

	if (hsName.size() > 0)
	{
		ID3DBlob* hull = nullptr;
		result = D3DCompileFromFile(path, nullptr, &hlslinculde,
			hsName.data(), "hs_5_0", compileflag, 0, &hull, &err);
		outErr(err);
		mDatas[shaderPath].hullShader.Swap(hull);
	}

	if (dsName.size() > 0)
	{
		ID3DBlob* domain = nullptr;
		result = D3DCompileFromFile(path, nullptr, &hlslinculde,
			dsName.data(), "ds_5_0", compileflag, 0, &domain, &err);
		outErr(err);
		mDatas[shaderPath].domainShader.Swap (domain);
	}

	return mDatas[shaderPath];
}

void ShaderCompiler::ReleaseShader(std::string shaderpath)
{
	mDatas.erase(shaderpath);
}