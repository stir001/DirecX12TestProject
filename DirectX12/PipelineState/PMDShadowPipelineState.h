#pragma once
#include "PipelineStateObject.h"
#include <memory>

class RootSignatureObject;

class PMDShadowPipelineState :
	public PipelineStateObject
{
public:
	PMDShadowPipelineState(std::shared_ptr<RootSignatureObject>& rootsignature,
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~PMDShadowPipelineState();
};

