#include "SwapChainComponent.h"
#include "RTVDescriptorHeapObject.h"
#include "Dx12Ctrl.h"
#include <d3d12.h>
#include "d3dx12.h"
#include <dxgi1_4.h>


SwapChainComponent::SwapChainComponent(HWND& hwnd):swapchain(nullptr),rtvDescriptorHeap(nullptr)
{
	Dx12Ctrl* d12 = Dx12Ctrl::Instance();
	d12->result = d12->GetFactory()->CreateSwapChainForHwnd(d12->GetCmdQueue(), hwnd, &(d12->GetDefaultSwapChainDesc()), nullptr, nullptr, (IDXGISwapChain1**)(&swapchain));
	D12RESULTCHECK
	DXGI_SWAP_CHAIN_DESC swdesc = {};
	swapchain->GetDesc(&swdesc);
	rtvDescriptorHeap = new RTVDescriptorHeapObject(swdesc.BufferCount);
	heapIncrementsize = d12->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	renderTargets.resize(swdesc.BufferCount);
	D3D12_CPU_DESCRIPTOR_HANDLE cpuhandle = rtvDescriptorHeap->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	for (int i = 0; i < renderTargets.size(); i++)
	{
		d12->result = swapchain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i]));
		D12RESULTCHECK
		d12->GetDev()->CreateRenderTargetView(renderTargets[i], nullptr, cpuhandle);
		cpuhandle.ptr += (heapIncrementsize);
	}

	bbindex = swapchain->GetCurrentBackBufferIndex();
}


SwapChainComponent::~SwapChainComponent()
{
	swapchain->Release();
	delete rtvDescriptorHeap;
	for (auto rt : renderTargets) rt->Release();
}

IDXGISwapChain3* SwapChainComponent::GetSwapChain()
{
	return swapchain;
}

D3D12_CPU_DESCRIPTOR_HANDLE SwapChainComponent::GetCurrentRTVHeap()
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtn;
	rtn.ptr = rtvDescriptorHeap->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart().ptr + bbindex * heapIncrementsize;
	return rtn;
}

ID3D12Resource* SwapChainComponent::GetCurrentRenderTarget()
{
	return renderTargets[bbindex];
}

void SwapChainComponent::SwapChainPresent(UINT SyncInterval, UINT flags)
{
	swapchain->Present(SyncInterval, flags);
	bbindex = swapchain->GetCurrentBackBufferIndex();
}

void SwapChainComponent::Release()
{
	swapchain->Release();
	rtvDescriptorHeap->GetDescriptorHeap()->Release();
	for (auto& rt : renderTargets)
	{
		rt->Release();
	}
}