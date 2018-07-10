#include "PipelineStateObject.h"


PipelineStateObject::PipelineStateObject(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, Microsoft::WRL::ComPtr<ID3D12Device> dev)
{
	dev->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&pipelineState));
}

PipelineStateObject::PipelineStateObject():pipelineState(nullptr)
{
}

PipelineStateObject::~PipelineStateObject()
{
	pipelineState.Reset();
}

Microsoft::WRL::ComPtr<ID3D12PipelineState> PipelineStateObject::GetPipelineState()
{
	return pipelineState;
}

void PipelineStateObject::CreatePipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, Microsoft::WRL::ComPtr<ID3D12Device> dev)
{
	if (pipelineState != nullptr) return;
	dev->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&pipelineState));
}