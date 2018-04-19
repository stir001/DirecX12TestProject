#pragma once
#include "DescriptorHeapObject.h"
class RTVDescriptorHeapObject :
	public DescriptorHeapObject
{
	ID3D12DescriptorHeap* rtvDescriptorHeap;

public:
	ID3D12DescriptorHeap* GetDescriptorHeap();

	RTVDescriptorHeapObject();
	RTVDescriptorHeapObject(int numDescriptor);
	~RTVDescriptorHeapObject();
};

