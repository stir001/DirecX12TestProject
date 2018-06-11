#include "SRVDescriptorHeapObject.h"
#include "Dx12Ctrl.h"



SRVDescriptorHeapObject::SRVDescriptorHeapObject()
{
	Dx12Ctrl::Instance().GetDev()->CreateDescriptorHeap(&(Dx12Ctrl::Instance().MakeDescriptorHeapDesc(dhdt_srv)), IID_PPV_ARGS(&srvDescriptorHeap));
}

SRVDescriptorHeapObject::SRVDescriptorHeapObject(int numDescriptor)
{
	Dx12Ctrl& d12= Dx12Ctrl::Instance();
	D3D12_DESCRIPTOR_HEAP_DESC desc = d12.MakeDescriptorHeapDesc(dhdt_srv, numDescriptor);
	d12.result = d12.GetDev()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvDescriptorHeap));
	D12RESULTCHECK
}

SRVDescriptorHeapObject::~SRVDescriptorHeapObject()
{
	srvDescriptorHeap->Release();
}

ID3D12DescriptorHeap* SRVDescriptorHeapObject::GetDescriptorHeap()
{
	return srvDescriptorHeap;
}
