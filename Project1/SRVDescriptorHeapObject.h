#pragma once
#include "DescriptorHeapObject.h"
class SRVDescriptorHeapObject :
	public DescriptorHeapObject
{
private:
	ID3D12DescriptorHeap* srvDescriptorHeap;
public:
	ID3D12DescriptorHeap* GetDescriptorHeap();
	SRVDescriptorHeapObject();
	SRVDescriptorHeapObject(int numDescriptor);
	~SRVDescriptorHeapObject();
};

