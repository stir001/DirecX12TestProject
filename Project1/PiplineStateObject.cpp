#include "PiplineStateObject.h"
#include "Dx12Ctrl.h"


PiplineStateObject::PiplineStateObject(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc)
{
	DX12CTRL_INSTANCE
	d12->result = d12->GetDev()->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&piplineState));
	D12RESULTCHECK
}

PiplineStateObject::PiplineStateObject():piplineState(nullptr)
{

}

PiplineStateObject::~PiplineStateObject()
{
	piplineState->Release();
}

ID3D12PipelineState* PiplineStateObject::GetPiplineState()
{
	return piplineState;
}

void PiplineStateObject::CreatePiplineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc)
{
	if (piplineState != nullptr) return;
	DX12CTRL_INSTANCE
		d12->result = d12->GetDev()->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&piplineState));
	D12RESULTCHECK
}