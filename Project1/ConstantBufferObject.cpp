#include "ConstantBufferObject.h"
#include "Dx12Ctrl.h"


ConstantBufferObject::ConstantBufferObject(unsigned int elementsize,unsigned int elementcount) : cmdList(nullptr), cbvDescHeap(nullptr)
{
	DX12CTRL_INSTANCE
	D3D12_HEAP_PROPERTIES cbvHeapProp = {};
	D3D12_RESOURCE_DESC cbvResourceDesc = {};

	cbvHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	cbvHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	cbvHeapProp.CreationNodeMask = 1;
	cbvHeapProp.VisibleNodeMask = 1;
	cbvHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	cbvResourceDesc.DepthOrArraySize = 1;
	cbvResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbvResourceDesc.MipLevels = 1;
	cbvResourceDesc.Width = elementcount * ((elementsize + 0xff) & ~0xff);
	cbvResourceDesc.Height = 1;
	cbvResourceDesc.SampleDesc.Count = 1;
	cbvResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	d12->result = d12->GetDev()->CreateCommittedResource(
		&cbvHeapProp,
		D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&cbvResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buffer)
	);
	D12RESULTCHECK
		
	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.NumDescriptors = elementcount;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	d12->result = d12->GetDev()->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&cbvDescHeap));
	D12RESULTCHECK

	auto bufferLocation = buffer->GetGPUVirtualAddress();
	auto descSize = d12->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	auto handle = cbvDescHeap->GetCPUDescriptorHandleForHeapStart();
	for (unsigned int i = 0; i < elementcount; i++)
	{
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = bufferLocation;
		cbvDesc.SizeInBytes = (elementsize + 0xff) & ~0xff;
		d12->GetDev()->CreateConstantBufferView(&cbvDesc, handle);
		bufferLocation += (elementsize + 0xff) & ~0xff;
		handle.ptr += descSize;
	}

	Map();
}


ConstantBufferObject::~ConstantBufferObject()
{
	cbvDescHeap->Release();
	cmdList->Release();
}

D3D12_GPU_DESCRIPTOR_HANDLE ConstantBufferObject::GetGPUDescriptorHandle()
{
	return cbvDescHeap->GetGPUDescriptorHandleForHeapStart();
}

void ConstantBufferObject::SetDescHeap()
{
	DX12CTRL_INSTANCE
	d12->GetCmdList()->SetDescriptorHeaps(1, &cbvDescHeap);
}

void ConstantBufferObject::SetRootparameterIndex(int index)
{
	rootpramIndex = index;
}

void ConstantBufferObject::SetBuffer()
{
	cmdList->SetGraphicsRootDescriptorTable(rootpramIndex, GetGPUDescriptorHandle());
}

void ConstantBufferObject::SetCommandList(ID3D12GraphicsCommandList* incmdList)
{
	if (cmdList != nullptr) cmdList->Release();
	cmdList = incmdList;
	incmdList->AddRef();
}