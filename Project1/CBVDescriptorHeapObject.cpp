#include "CBVDescriptorHeapObject.h"
#include "Dx12Ctrl.h"
#include <d3d12.h>


CBVDescriptorHeapObject::CBVDescriptorHeapObject()
{
	Dx12Ctrl::Instance().GetDev()->CreateDescriptorHeap(&(Dx12Ctrl::Instance().MakeDescriptorHeapDesc(dhdt_cbv)), IID_PPV_ARGS(&cbvDescriptorHeap));
}

CBVDescriptorHeapObject::CBVDescriptorHeapObject(int numDescriptor)
{
	Dx12Ctrl::Instance().GetDev()->CreateDescriptorHeap(&(Dx12Ctrl::Instance().MakeDescriptorHeapDesc(dhdt_cbv, numDescriptor)), IID_PPV_ARGS(&cbvDescriptorHeap));
}

CBVDescriptorHeapObject::~CBVDescriptorHeapObject()
{
	cbvDescriptorHeap->Release();
}

ID3D12DescriptorHeap* CBVDescriptorHeapObject::GetDescriptorHeap()
{
	return cbvDescriptorHeap;
}

