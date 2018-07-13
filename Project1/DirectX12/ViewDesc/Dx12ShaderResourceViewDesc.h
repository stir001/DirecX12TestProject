#pragma once
#include "Dx12BufferViewDesc.h"
class Dx12ShaderResourceViewDesc :
	public Dx12BufferViewDesc
{
public:
	Dx12ShaderResourceViewDesc(DXGI_FORMAT format);
	~Dx12ShaderResourceViewDesc();

	void CreateView(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle,
		D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr);
private:
	D3D12_SHADER_RESOURCE_VIEW_DESC mSrvDesc;
};

