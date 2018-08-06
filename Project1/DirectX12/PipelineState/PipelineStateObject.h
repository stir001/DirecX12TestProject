#pragma once
#include "ShaderDatasStructure.h"

#include <d3d12.h>
#include <wrl.h>

class PipelineStateObject
{
public:
	PipelineStateObject();
	PipelineStateObject(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, Microsoft::WRL::ComPtr<ID3D12Device> dev);
	virtual ~PipelineStateObject();

	Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState() const;
	void CreatePipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, Microsoft::WRL::ComPtr<ID3D12Device> dev);
protected:
	void SetShaders(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, const ShaderDatas& shaders);

	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPipelineState;
};

