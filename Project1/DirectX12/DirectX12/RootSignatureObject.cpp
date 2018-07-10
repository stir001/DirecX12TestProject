#include "RootSignatureObject.h"
#include <d3d12.h>


RootSignatureObject::RootSignatureObject(ID3D10Blob* signatureBlob,Microsoft::WRL::ComPtr<ID3D12Device> dev)
{
	dev->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&mRootSignature));
}

RootSignatureObject::~RootSignatureObject()
{
	//mRootSignature.Reset();
}

Microsoft::WRL::ComPtr<ID3D12RootSignature> RootSignatureObject::GetRootSignature()
{
	return mRootSignature;
}

