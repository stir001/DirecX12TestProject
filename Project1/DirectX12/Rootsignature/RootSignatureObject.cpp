#include "RootSignatureObject.h"
#include <d3d12.h>

RootSignatureObject::RootSignatureObject(): mRootSignature(nullptr)
{
}

RootSignatureObject::RootSignatureObject(ID3D10Blob* signatureBlob,Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	CreateRootSignature(signatureBlob, dev);
}

RootSignatureObject::~RootSignatureObject()
{
	mRootSignature.Reset();
}

void RootSignatureObject::SetShaderData(const ShaderDatas& shader)
{
	mShader = shader;
}

ShaderDatas& RootSignatureObject::GetShaderDatas()
{
	return mShader;
}

Microsoft::WRL::ComPtr<ID3D12RootSignature>& RootSignatureObject::GetRootSignature()
{
	return mRootSignature;
}

void RootSignatureObject::CreateRootSignature(ID3D10Blob * signatureBlob, Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	dev->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&mRootSignature));
}

