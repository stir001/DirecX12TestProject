#pragma once
#include "DirectX12Buffer.h"

class ConstantBufferObject :
	public DirectX12Buffer
{
private:
	ID3D12DescriptorHeap* cbvDescHeap;
	ID3D12GraphicsCommandList* cmdList;
	int rootpramIndex;
public:
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle();
	void SetDescHeap();

	void SetRootparameterIndex(int index);
	void SetBuffer();
	void SetCommandList(ID3D12GraphicsCommandList* incmdList);

	ConstantBufferObject(unsigned int elementsize, unsigned int elementcount);
	~ConstantBufferObject();
};

