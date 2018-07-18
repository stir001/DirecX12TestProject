#include "Dx12BufferObject.h"
#include "Dx12ConstantBufferViewDesc.h"
#include "Dx12ShaderResourceViewDesc.h"
#include "Dx12DepthStencilViewDesc.h"
#include "Dx12RenderTargetViewDesc.h"
#include "Dx12UnorderedAccessViewDesc.h"
#include "CharToWChar.h"
#include "Dx12Ctrl.h"


Dx12BufferObject::Dx12BufferObject(const std::string& name) : mBuffer(nullptr),mElementBuffer(nullptr)
{
	wchar_t* buf = nullptr;
	ToWChar(&buf, name);
	mName = buf;
	delete buf;
}

Dx12BufferObject::Dx12BufferObject(Microsoft::WRL::ComPtr<ID3D12Resource>& rsc, const std::string& name,
	unsigned int elemenetSize, unsigned int elementCount):mBuffer(rsc),mElementSize(elemenetSize),mElementCount(elementCount)
{
#ifdef _DEBUG
	mRscDesc = rsc->GetDesc();
	D3D12_HEAP_FLAGS flag;
	rsc->GetHeapProperties(&mHeapProp, &flag);
#endif // _DEBUG

	wchar_t* buf = nullptr;
	ToWChar(&buf, name);
	mName = buf;
	mBuffer->SetName(mName.data());
	delete buf;
}

Dx12BufferObject::~Dx12BufferObject()
{
	if (mElementBuffer != nullptr)
	{
		mBuffer->Unmap(0, nullptr);
	}
	mElementBuffer = nullptr;


	//https://docs.microsoft.com/en-us/windows/desktop/api/d3d12/ns-d3d12-d3d12_discard_region
	D3D12_DISCARD_REGION region;
	if (mBuffer->GetDesc().Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)
	{
		region.NumRects = 1;
	}
	else
	{
		region.NumRects = 0;
	}

	D3D12_RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = static_cast<LONG>(mBuffer->GetDesc().Width);
	rect.bottom = static_cast<LONG>(mBuffer->GetDesc().Height);

	region.pRects = &rect;
	region.FirstSubresource = 0;
	region.NumSubresources = 1;

	Dx12Ctrl::Instance()->GetCmdList()->DiscardResource(mBuffer.Get(), &region);
	Dx12Ctrl::Instance()->CmdQueueSignal();
	unsigned long resetCount = mBuffer.Reset();
	mViewDescs.reset();
}

void Dx12BufferObject::WriteBuffer(const void* pData, unsigned int amountDatasSize)
{
	D3D12_RANGE range{ 0, amountDatasSize };
	mBuffer->Map(0, &range, &mElementBuffer);
	memcpy(mElementBuffer, pData, amountDatasSize);
	mBuffer->Unmap(0,&range);
	mElementBuffer = nullptr;
}

void Dx12BufferObject::Map()
{

}

void Dx12BufferObject::WriteBuffer256Alignment(const void* pData, unsigned int datasize, unsigned int datacount)
{
	D3D12_RANGE range{ 0, ((datasize + 0xff) & ~0xff) * datacount };
	mBuffer->Map(0, &range, &mElementBuffer);
	for (unsigned int i = 0; i < datacount; i++)
	{
		memcpy((void*)((char*)mElementBuffer + i * ((datasize + 0xff) & ~0xff)), (void*)((char*)pData + i * datasize), datasize);
	}
	mBuffer->Unmap(0, &range);
	mElementBuffer = nullptr;
}

Microsoft::WRL::ComPtr<ID3D12Resource>& Dx12BufferObject::GetBuffer()
{
	return mBuffer;
}

D3D12_RESOURCE_STATES Dx12BufferObject::GetCurrentState() const
{
	return mCurrentState;
}

void Dx12BufferObject::SetCurrentState(D3D12_RESOURCE_STATES state)
{
	mCurrentState = state;
}

D3D12_RESOURCE_STATES Dx12BufferObject::GetDefaultState() const
{
	return mDeafultState;
}

void Dx12BufferObject::CreateConstantBufferViewDesc()
{
	mViewDescs.reset(new Dx12ConstantBufferViewDesc(mBuffer->GetGPUVirtualAddress(),(mElementSize + 0xff) & ~0xff, mElementCount));
}

void Dx12BufferObject::CreateUnorderdAccessViewDesc()
{
	mViewDescs.reset(new Dx12UnorderedAccessViewDesc(mElementCount, mElementSize));
}

void Dx12BufferObject::CreateShaderResourceViewDesc()
{
	mViewDescs.reset(new Dx12ShaderResourceViewDesc(mBuffer->GetDesc().Format));
}

void Dx12BufferObject::CreateRenderTargetViewDesc()
{
	mViewDescs.reset(new Dx12RenderTargetViewDesc(mBuffer->GetDesc().Format));
}

void Dx12BufferObject::CreateDepthStecilViewDesc()
{
	mViewDescs.reset(new Dx12DepthStencilViewDesc(mBuffer->GetDesc().Format));
}

const std::shared_ptr<Dx12BufferViewDesc>& Dx12BufferObject::GetViewDesc() const
{
	return mViewDescs;
}

