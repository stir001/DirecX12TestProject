#include "VertexBufferObject.h"
#include "Dx12Ctrl.h"

VertexBufferObject::VertexBufferObject(unsigned int elementsize, unsigned int elementcount)
{
	DX12CTRL_INSTANCE
	d12->result = d12->GetDev()->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((elementcount * elementsize + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buffer));
	D12RESULTCHECK

	char* vdata = nullptr;
	d12->result = buffer->Map(0, nullptr, (void**)(&vdata));
	D12RESULTCHECK

	
	vbView.BufferLocation = buffer->GetGPUVirtualAddress();
	vbView.SizeInBytes = (elementcount * elementsize + 0xff) & ~0xff;
	vbView.StrideInBytes = elementsize;

	Map();
}


VertexBufferObject::~VertexBufferObject()
{
}

void VertexBufferObject::SetBuffer() const
{
	DX12CTRL_INSTANCE
	d12->GetCmdList()->IASetVertexBuffers(0, 1, &vbView);
}