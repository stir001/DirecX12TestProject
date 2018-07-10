#pragma once
#include "Dx12BufferViewDesc.h"
class Dx12UnorderedAccessViewDesc :
	public Dx12BufferViewDesc
{
public:
	Dx12UnorderedAccessViewDesc(UINT numElements, UINT stride);
	~Dx12UnorderedAccessViewDesc();

	void CreateView(Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, 
		D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr);
private:
	const D3D12_UNORDERED_ACCESS_VIEW_DESC mUavDesc;
};

