#include "RTVDescriptorHeapObject.h"
#include "Dx12Ctrl.h"

#include "d3dx12.h"
#include <d3d12.h>
#include <dxgi1_4.h>

RTVDescriptorHeapObject::RTVDescriptorHeapObject()
{
	Dx12Ctrl::Instance().GetDev()->CreateDescriptorHeap(&(Dx12Ctrl::Instance().MakeDescriptorHeapDesc(dhdt_rtv)), IID_PPV_ARGS(&rtvDescriptorHeap));
}

RTVDescriptorHeapObject::RTVDescriptorHeapObject(int numDescriptor)
{
	Dx12Ctrl::Instance().GetDev()->CreateDescriptorHeap(&(Dx12Ctrl::Instance().MakeDescriptorHeapDesc(dhdt_rtv,numDescriptor)), IID_PPV_ARGS(&rtvDescriptorHeap));
}


RTVDescriptorHeapObject::~RTVDescriptorHeapObject()
{
	rtvDescriptorHeap->Release();
}

ID3D12DescriptorHeap* RTVDescriptorHeapObject::GetDescriptorHeap()
{
	return rtvDescriptorHeap;
}
