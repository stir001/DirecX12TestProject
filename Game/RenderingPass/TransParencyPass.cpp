#include "TransParencyPass.h"



TransParencyPass::TransParencyPass(std::shared_ptr<Dx12DescriptorHeapObject>& rtvHeap,
	std::shared_ptr<Dx12DescriptorHeapObject>& depthHeap)
	: RenderingPassObject("TransParency"), mRTVDescHeap(rtvHeap)
{
	mCmdList = std::make_shared<Dx12CommandList>("TransParency", Dx12Ctrl::Instance().GetDev());
	
}


TransParencyPass::~TransParencyPass()
{
}

void TransParencyPass::FirstUpdate()
{
	mCmdList->OMSetRenderTargets(1, mRTVDescHeap->GetCPUHeapHandleStart(), &mDepthHeap->GetCPUHeapHandleStart());
}

void TransParencyPass::PreExecuteUpdate()
{
}

void TransParencyPass::ExecutePath(Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue)
{
	ID3D12CommandList* cmdList = mCmdList->GetCommandList().Get();
	cmdQueue->ExecuteCommandLists(1, &cmdList);
}

void TransParencyPass::ResetCommandList()
{
	mCmdList->Reset();
}

std::shared_ptr<Dx12CommandList> TransParencyPass::GetCommandList()
{
	return mCmdList;
}
