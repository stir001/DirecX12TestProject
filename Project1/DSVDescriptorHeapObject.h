#pragma once
#include "DescriptorHeapObject.h"
class DSVDescriptorHeapObject :
	public DescriptorHeapObject
{
private:
	ID3D12DescriptorHeap* dsvDescriptorHeap;
public:
	ID3D12DescriptorHeap* GetDescriptorHeap();
	DSVDescriptorHeapObject();
	DSVDescriptorHeapObject(int numDescriptor);
	~DSVDescriptorHeapObject();
};

