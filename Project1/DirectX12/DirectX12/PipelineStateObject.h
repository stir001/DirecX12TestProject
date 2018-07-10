#pragma once
#include <d3d12.h>
#include <wrl.h>

class PipelineStateObject
{
private:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
public:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState();
	void CreatePipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, Microsoft::WRL::ComPtr<ID3D12Device> dev);

	PipelineStateObject(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, Microsoft::WRL::ComPtr<ID3D12Device> dev);
	PipelineStateObject();
	~PipelineStateObject();
};

