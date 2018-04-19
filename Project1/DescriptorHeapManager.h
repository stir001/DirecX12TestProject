#pragma once
#include <vector>

class DSVDescriptorHeapObject;
class CBVDescriptorHeapObject;
class RTVDescriptorHeapObject;
class SRVDescriptorHeapObject;
class RootSignatureObject;

class DescriptorHeapManager
{
private:
	RootSignatureObject* rootsignature;
	std::vector<DSVDescriptorHeapObject> dsvHeaps;
	std::vector<CBVDescriptorHeapObject> cbvHeaps;
	std::vector<RTVDescriptorHeapObject> rtvHeaps;
	std::vector<SRVDescriptorHeapObject> srvHeaps;
public:
	void SetRootSignatureObject(RootSignatureObject* root);

	void AddDsvHeap(DSVDescriptorHeapObject indsv);
	void AddCbvHeap(CBVDescriptorHeapObject incbv);
	void AddRtvHeap(RTVDescriptorHeapObject inrtv);
	void AddSrvHeap(SRVDescriptorHeapObject insrv);

	std::vector<DSVDescriptorHeapObject>& GetDsvHeap();
	std::vector<CBVDescriptorHeapObject>& GetCbvHeap();
	std::vector<RTVDescriptorHeapObject>& GetRtvHeap();
	std::vector<SRVDescriptorHeapObject>& GetSrvHeap();
	DescriptorHeapManager();
	~DescriptorHeapManager();
};

