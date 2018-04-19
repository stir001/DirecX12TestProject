#pragma once
#include "DirectX12Buffer.h"

#include <d3d12.h>

class DepthBufferObject :
	public DirectX12Buffer
{
private:
	ID3D12DescriptorHeap* dsvHeap;
	void Map();
public:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUAdress();
	DepthBufferObject(int height, int width);
	~DepthBufferObject();
};

