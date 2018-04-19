#include "DescriptorHeapManager.h"
#include "RootSignatureObject.h"
#include "DSVDescriptorHeapObject.h"
#include "CBVDescriptorHeapObject.h"
#include "RTVDescriptorHeapObject.h"
#include "SRVDescriptorHeapObject.h"

#include <d3d12.h>



DescriptorHeapManager::DescriptorHeapManager()
{
}


DescriptorHeapManager::~DescriptorHeapManager()
{
}

void DescriptorHeapManager::SetRootSignatureObject(RootSignatureObject* root)
{
	rootsignature = root;
}

void DescriptorHeapManager::AddDsvHeap(DSVDescriptorHeapObject indsv)
{
	dsvHeaps.push_back(indsv);
}

void DescriptorHeapManager::AddCbvHeap(CBVDescriptorHeapObject incbv)
{
	cbvHeaps.push_back(incbv);
}

void DescriptorHeapManager::AddRtvHeap(RTVDescriptorHeapObject inrtv)
{
	rtvHeaps.push_back(inrtv);
}

void DescriptorHeapManager::AddSrvHeap(SRVDescriptorHeapObject insrv)
{
	srvHeaps.push_back(insrv);
}

std::vector<DSVDescriptorHeapObject>& DescriptorHeapManager::GetDsvHeap()
{
	return dsvHeaps;
}
std::vector<CBVDescriptorHeapObject>& DescriptorHeapManager::GetCbvHeap()
{
	return cbvHeaps;
}
std::vector<RTVDescriptorHeapObject>& DescriptorHeapManager::GetRtvHeap()
{
	return rtvHeaps;
}
std::vector<SRVDescriptorHeapObject>& DescriptorHeapManager::GetSrvHeap()
{
	return srvHeaps;
}