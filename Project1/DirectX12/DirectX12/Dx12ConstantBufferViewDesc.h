#pragma once
#include "Dx12BufferViewDesc.h"
class Dx12ConstantBufferViewDesc :
	public Dx12BufferViewDesc
{
public:
	Dx12ConstantBufferViewDesc(D3D12_GPU_VIRTUAL_ADDRESS gpuVirtualAddress, UINT sizeInBytes, unsigned int elementCount);
	~Dx12ConstantBufferViewDesc();
	void CreateView(Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, 
		D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, Microsoft::WRL::ComPtr<ID3D12Resource> resource);

	unsigned int GetViewDescCount() const;
private:
	const D3D12_CONSTANT_BUFFER_VIEW_DESC mCbvView;
	unsigned int mElementCount;
};

