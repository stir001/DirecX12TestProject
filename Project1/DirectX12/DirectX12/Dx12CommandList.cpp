#include "Dx12CommandList.h"
#include "Dx12BufferObject.h"
#include "Dx12Ctrl.h"
#include "Dx12DescriptorHeapObject.h"
#include "CharToWChar.h"


Dx12CommandList::Dx12CommandList(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_COMMAND_LIST_TYPE type)
	:mType(type)
{
	wchar_t* buf = nullptr;
	ToWChar(&buf, name);
	std::wstring comName = buf;
	comName += L"Allocator";
	dev->CreateCommandAllocator(type, IID_PPV_ARGS(&mCmdallcator));

	comName = buf;
	comName += L"CommandList";
	dev->CreateCommandList(0, mType ,mCmdallcator.Get(),nullptr,IID_PPV_ARGS(&mCmdList));

	delete buf;

	mName = name;
}


Dx12CommandList::~Dx12CommandList()
{
	ULONG count = mCmdList.Reset();
	count = mCmdallcator.Reset();
}

const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& Dx12CommandList::GetCommandList() const
{
	return mCmdList;
}

D3D12_COMMAND_LIST_TYPE Dx12CommandList::GetType() const
{
	return mType;
}

HRESULT Dx12CommandList::Reset()
{
	mCmdallcator->Reset();
	return mCmdList->Reset(mCmdallcator.Get(), nullptr);
}

HRESULT Dx12CommandList::Reset(Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState)
{
	mCmdallcator->Reset();
	return mCmdList->Reset(mCmdallcator.Get(), pipelineState.Get());
}

HRESULT Dx12CommandList::SetDescriptorHeap(const std::shared_ptr<Dx12DescriptorHeapObject>& descHeap) const
{
	HRESULT rtn = WSAEINVAL;
	mCmdList->SetDescriptorHeaps(1, descHeap->GetDescriptorHeap().GetAddressOf());

#ifdef _DEBUG
	rtn = Dx12Ctrl::Instance()->GetDeviceRemoveReason();
#else
	rtn = S_OK;
#endif

	return E_NOTIMPL;
}

HRESULT Dx12CommandList::TransitionBarrier(std::shared_ptr<Dx12BufferObject>& resource, D3D12_RESOURCE_STATES transitionState) const
{
	HRESULT rtn = WSAEINVAL;
	if (resource->GetCurrentState() == transitionState)
	{
		return rtn;
	}

	D3D12_RESOURCE_BARRIER barrier;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = resource->GetBuffer().Get();
	barrier.Transition.StateAfter = transitionState;
	barrier.Transition.StateBefore = resource->GetCurrentState();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	mCmdList->ResourceBarrier(1, &barrier);

	resource->SetCurrentState(transitionState);

#ifdef _DEBUG
	rtn = Dx12Ctrl::Instance()->GetDeviceRemoveReason();
#else
	rtn = S_OK;
#endif
	return rtn;
}

const std::string & Dx12CommandList::GetName() const
{
	return mName;
}

void Dx12CommandList::Close() const
{
	mCmdList->Close();
}

