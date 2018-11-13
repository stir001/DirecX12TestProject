#include "stdafx.h"
#include "FirstPathObject.h"
#include "CommandList/Dx12CommandList.h"
#include "Buffer/DepthBufferObject.h"
#include "Buffer/RendertargetObject.h"
#include <d3d12.h>

FirstPathObject::FirstPathObject(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, std::shared_ptr<DepthBufferObject> depthBuffer,
	unsigned int wWidth, unsigned int wHeight): 
	mDepthBuffer(depthBuffer), 
	mRendertarget(std::make_shared<RendertargetObject>("FirstpathRendertarget", dev, wWidth, wHeight))
{
	mCmdList = std::make_shared<Dx12CommandList>("FirstPath", dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
}


FirstPathObject::~FirstPathObject()
{
}

void FirstPathObject::FirstUpdate()
{
}

void FirstPathObject::PreExcutePath()
{
}

void FirstPathObject::ExcutePath(Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue)
{
	cmdQueue->ExecuteCommandLists(1, (ID3D12CommandList**)(mCmdList->GetCommandList().GetAddressOf()));
}

void FirstPathObject::ResetCommandList()
{
	mCmdList->Reset();
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> FirstPathObject::GetCommandList()
{
	return mCmdList->GetCommandList();
}

Microsoft::WRL::ComPtr<ID3D12Resource> FirstPathObject::GetRenderTarget()
{
	return Microsoft::WRL::ComPtr<ID3D12Resource>();
}
