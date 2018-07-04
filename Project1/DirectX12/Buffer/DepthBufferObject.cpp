#include "DepthBufferObject.h"
#include "Dx12Getter.h"
#include "Dx12Ctrl.h"
#include "Dx12ResultCheckDefine.h"
#include <cassert>

DepthBufferObject::DepthBufferObject(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev, 
	int height,int width, DXGI_FORMAT format)
	:Dx12BufferObject(name)
{
	D3D12_RESOURCE_DESC rscDesc;
	rscDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	rscDesc.Width = height;
	rscDesc.Height = width;
	rscDesc.DepthOrArraySize = 1;
	rscDesc.Format = format;
	rscDesc.SampleDesc.Count = 1;
	rscDesc.SampleDesc.Quality = 0;
	rscDesc.MipLevels = 0;
	rscDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	rscDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	rscDesc.Alignment = 0;

	mClearValue = {}; 
	mClearValue.Format = format;
	mClearValue.DepthStencil.Depth = 1.0f;
	mClearValue.DepthStencil.Stencil = 0;

	D3D12_HEAP_PROPERTIES heapProp;
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProp.CreationNodeMask = 0;
	heapProp.VisibleNodeMask = 0;

#ifdef _DEBUG
	mRscDesc = rscDesc;
	mHeapProp = heapProp;
#endif

	mDeafultState = mCurrentState = D3D12_RESOURCE_STATE_DEPTH_WRITE;

	HRESULT result = S_OK;

	result = dev->CreateCommittedResource(&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&rscDesc,
		mDeafultState,
		&mClearValue,
		IID_PPV_ARGS(&mBuffer));

	D12RESULTCHECK(result, dev);

	mBuffer->SetName(mName.data());

	mElementCount = height * width;
	mElementSize = dx12_getter::GetDxgiFormatByteSize(format);

	CreateDepthStecilViewDesc();
}

DepthBufferObject::DepthBufferObject(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Resource>& rsc, D3D12_RESOURCE_STATES state)
	:Dx12BufferObject(rsc, name, 
		dx12_getter::GetDxgiFormatByteSize(rsc->GetDesc().Format),
		static_cast<unsigned int>(rsc->GetDesc().Height * rsc->GetDesc().Width))
{
	mDeafultState = mCurrentState = state;
	CreateDepthStecilViewDesc();
}

DepthBufferObject::~DepthBufferObject()
{
}

void DepthBufferObject::Map()
{
}

//D3D12_CPU_DESCRIPTOR_HANDLE DepthBufferObject::GetCPUAdress()
//{
//	return mViewCpuHandle;
//}

Microsoft::WRL::ComPtr<ID3D12Resource> DepthBufferObject::GetDepthBuffer()
{
	return mBuffer;
}