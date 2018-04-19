#pragma once
#include <d3d12.h>

class PiplineStateObject
{
private:
	ID3D12PipelineState* piplineState;
public:
	ID3D12PipelineState* GetPiplineState();
	void CreatePiplineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc);

	PiplineStateObject(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc);
	PiplineStateObject();
	~PiplineStateObject();
};

