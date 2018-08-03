#pragma once
#include "ShaderDatasStructure.h"

#include <vector>
#include <d3dcommon.h>
#include <wrl.h>
#include <d3d12.h>

struct ID3D12RootSignature;
struct ID3D10Blob;

class RootSignatureObject
{
public:
	RootSignatureObject();
	RootSignatureObject(ID3D10Blob* signatureBlob, Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~RootSignatureObject();

	void SetShaderData(const ShaderDatas& shader);
	ShaderDatas& GetShaderDatas();
	Microsoft::WRL::ComPtr<ID3D12RootSignature>& GetRootSignature();
protected:
	void CreateRootSignature(ID3D10Blob* signatureBlob, Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature;
	ShaderDatas mShader;
};

