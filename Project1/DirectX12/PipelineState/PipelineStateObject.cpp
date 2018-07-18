#include "PipelineStateObject.h"


PipelineStateObject::PipelineStateObject(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, Microsoft::WRL::ComPtr<ID3D12Device> dev)
{
	HRESULT hr = dev->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&mPipelineState));
}

PipelineStateObject::PipelineStateObject():mPipelineState(nullptr)
{
}

PipelineStateObject::~PipelineStateObject()
{
	mPipelineState.Reset();
}

Microsoft::WRL::ComPtr<ID3D12PipelineState> PipelineStateObject::GetPipelineState() const
{
	return mPipelineState;
}

void PipelineStateObject::CreatePipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, Microsoft::WRL::ComPtr<ID3D12Device> dev)
{
	if (mPipelineState != nullptr) return;
	dev->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&mPipelineState));
}