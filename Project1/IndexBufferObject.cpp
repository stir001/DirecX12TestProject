#include "IndexBufferObject.h"
#include "Dx12Ctrl.h"

#include "d3dx12.h"



IndexBufferObject::IndexBufferObject(unsigned int elementsize,unsigned int elementcount)
{
	DX12CTRL_INSTANCE
	d12->result = d12->GetDev()->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((elementsize * elementcount + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buffer));
	D12RESULTCHECK

	indexBufferView.BufferLocation = buffer->GetGPUVirtualAddress();
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	indexBufferView.SizeInBytes = (elementsize * elementcount + 0xff) & ~0xff;

	Map();
}


IndexBufferObject::~IndexBufferObject()
{
}

void IndexBufferObject::SetBuffer()
{
	DX12CTRL_INSTANCE
	d12->GetCmdList()->IASetIndexBuffer(&indexBufferView);
	d12->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}