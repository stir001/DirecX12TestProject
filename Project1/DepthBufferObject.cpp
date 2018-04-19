#include "DepthBufferObject.h"
#include "Dx12Ctrl.h"


DepthBufferObject::DepthBufferObject(int height,int width) 
{
	DX12CTRL_INSTANCE
	D3D12_RESOURCE_DESC depthResourceDesc = {};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = height;
	depthResourceDesc.Height = width;
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE depthClearValue = {};
	depthClearValue.DepthStencil.Depth = 1.0f;//[‚³Å‘å’l‚Í‚P 
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;

	d12->result = d12->GetDev()->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&buffer));
	D12RESULTCHECK


	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {}; 
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	d12->result = d12->GetDev()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	D12RESULTCHECK

	d12->GetDev()->CreateDepthStencilView(buffer, &dsvDesc, dsvHeap->GetCPUDescriptorHandleForHeapStart());

}


DepthBufferObject::~DepthBufferObject()
{
	DirectX12Buffer::~DirectX12Buffer();
	dsvHeap->Release();
}

void DepthBufferObject::Map()
{
}

D3D12_CPU_DESCRIPTOR_HANDLE DepthBufferObject::GetCPUAdress()
{
	return dsvHeap->GetCPUDescriptorHandleForHeapStart();
}

