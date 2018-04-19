#pragma once
struct 	ID3D12DescriptorHeap;


class DescriptorHeapObject
{
public:
	virtual ID3D12DescriptorHeap* GetDescriptorHeap() = 0;
	DescriptorHeapObject();
	~DescriptorHeapObject();
};

