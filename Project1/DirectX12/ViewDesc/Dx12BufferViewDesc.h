#pragma once
#include <d3d12.h>
#include <wrl.h>

class Dx12BufferViewDesc
{
public:
	Dx12BufferViewDesc();
	virtual ~Dx12BufferViewDesc();

	virtual unsigned int GetViewDescCount() const;
	virtual void CreateView(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, 
		D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr) = 0;
};

