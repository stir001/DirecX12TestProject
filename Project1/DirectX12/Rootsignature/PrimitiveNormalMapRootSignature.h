#pragma once
#include "RootSignatureObject.h"
class PrimitiveNormalMapRootSignature :
	public RootSignatureObject
{
public:
	PrimitiveNormalMapRootSignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~PrimitiveNormalMapRootSignature();
};

