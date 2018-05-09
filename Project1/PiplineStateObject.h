#pragma once
#include <d3d12.h>
#include <wrl.h>

class PiplineStateObject
{
private:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> piplineState;
public:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPiplineState();
	void CreatePiplineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc);

	PiplineStateObject(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc);
	PiplineStateObject();
	~PiplineStateObject();
};

