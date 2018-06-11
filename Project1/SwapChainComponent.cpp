#include "SwapChainComponent.h"
#include "RTVDescriptorHeapObject.h"
#include "Dx12Ctrl.h"
#include <d3d12.h>
#include "d3dx12.h"
#include <dxgi1_4.h>


SwapChainComponent::SwapChainComponent(HWND& hwnd):swapchain(nullptr),rtvDescriptorHeap(nullptr)
{
	Dx12Ctrl& d12 = Dx12Ctrl::Instance();
	auto swap = swapchain.Get();
	d12.result = d12.GetFactory()->CreateSwapChainForHwnd(d12.GetCmdQueue().Get(), hwnd, &(d12.GetDefaultSwapChainDesc()), nullptr, nullptr, (IDXGISwapChain1**)(&swap));
	D12RESULTCHECK
	swapchain = swap;
	DXGI_SWAP_CHAIN_DESC swdesc = {};
	swapchain->GetDesc(&swdesc);
	rtvDescriptorHeap = new RTVDescriptorHeapObject(swdesc.BufferCount);
	heapIncrementsize = d12.GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	renderTargets.resize(swdesc.BufferCount);
	D3D12_CPU_DESCRIPTOR_HANDLE cpuhandle = rtvDescriptorHeap->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	D3D12_RENDER_TARGET_VIEW_DESC rtdesc;
	rtdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtdesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtdesc.Texture2D.MipSlice = 0;
	rtdesc.Texture2D.PlaneSlice = 0;
	
	for (int i = 0; i < renderTargets.size(); i++)
	{
		d12.result = swapchain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i]));
		D12RESULTCHECK
		d12.GetDev()->CreateRenderTargetView(renderTargets[i], &rtdesc, cpuhandle);
		cpuhandle.ptr += (heapIncrementsize);
	}

	bbindex = swapchain->GetCurrentBackBufferIndex();
}


SwapChainComponent::~SwapChainComponent()
{
	delete rtvDescriptorHeap;
	for (auto rt : renderTargets) rt->Release();
}

Microsoft::WRL::ComPtr<IDXGISwapChain3> SwapChainComponent::GetSwapChain()
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
	rtvDescriptorHeap->GetDescriptorHeap()->Release();
	for (auto& rt : renderTargets)
	{
		rt->Release();
	}
}