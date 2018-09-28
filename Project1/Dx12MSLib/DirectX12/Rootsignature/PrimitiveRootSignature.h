#pragma once
#include "RootSignatureObject.h"

class PrimitiveRootSignature :
	public RootSignatureObject
{
public:
	PrimitiveRootSignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~PrimitiveRootSignature();
};

