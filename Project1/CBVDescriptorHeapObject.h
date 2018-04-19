#pragma once
#include "DescriptorHeapObject.h"
class CBVDescriptorHeapObject :
	public DescriptorHeapObject
{
private:
	ID3D12DescriptorHeap* cbvDescriptorHeap;
public:
	ID3D12DescriptorHeap* GetDescriptorHeap();
	CBVDescriptorHeapObject();
	CBVDescriptorHeapObject(int numDescriptor);
	~CBVDescriptorHeapObject();
};

