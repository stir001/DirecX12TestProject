#include "PipelineStateObject.h"
#include "d3dx12.h"

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

void PipelineStateObject::SetShaders(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, const ShaderDatas& shaders)
{
	if (shaders.vertexShader != nullptr)
	{
		gpsDesc.VS = CD3DX12_SHADER_BYTECODE(shaders.vertexShader.Get());
	}

	if (shaders.pixelShader != nullptr)
	{
		gpsDesc.PS = CD3DX12_SHADER_BYTECODE(shaders.pixelShader.Get());
	}

	if (shaders.geometryShader != nullptr)
	{
		gpsDesc.GS = CD3DX12_SHADER_BYTECODE(shaders.geometryShader.Get());
	}

	if (shaders.domainShader != nullptr)
	{
		gpsDesc.DS = CD3DX12_SHADER_BYTECODE(shaders.domainShader.Get());
	}

	if (shaders.hullShader != nullptr)
	{
		gpsDesc.HS = CD3DX12_SHADER_BYTECODE(shaders.hullShader.Get());
	}
}
