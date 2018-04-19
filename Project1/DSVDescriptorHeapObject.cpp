#include "DSVDescriptorHeapObject.h"
#include "Dx12Ctrl.h"
#include <d3d12.h>



DSVDescriptorHeapObject::DSVDescriptorHeapObject()
{
	Dx12Ctrl::Instance()->GetDev()->CreateDescriptorHeap(&(Dx12Ctrl::Instance()->MakeDescriptorHeapDesc(dhdt_dsv)), IID_PPV_ARGS(&dsvDescriptorHeap));
}

DSVDescriptorHeapObject::DSVDescriptorHeapObject(int numDescriptor)
{
	Dx12Ctrl::Instance()->GetDev()->CreateDescriptorHeap(&(Dx12Ctrl::Instance()->MakeDescriptorHeapDesc(dhdt_dsv,numDescriptor)), IID_PPV_ARGS(&dsvDescriptorHeap));
}


DSVDescriptorHeapObject::~DSVDescriptorHeapObject()
{
	dsvDescriptorHeap->Release();
}

ID3D12DescriptorHeap* DSVDescriptorHeapObject::GetDescriptorHeap()
{
	return dsvDescriptorHeap;
}