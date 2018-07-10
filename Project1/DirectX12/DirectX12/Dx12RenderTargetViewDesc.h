#pragma once
#include "Dx12BufferViewDesc.h"
class Dx12RenderTargetViewDesc :
	public Dx12BufferViewDesc
{
public:
	Dx12RenderTargetViewDesc(DXGI_FORMAT format);
	~Dx12RenderTargetViewDesc();

	void CreateView(Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, 
		D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, Microsoft::WRL::ComPtr<ID3D12Resource> resource);
private:
	D3D12_RENDER_TARGET_VIEW_DESC mRtvDesc;
};

