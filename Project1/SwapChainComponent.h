#pragma once
#include <vector>
#include <dxgi1_4.h>
#include <d3d12.h>


class RTVDescriptorHeapObject;

class SwapChainComponent
{
private:
	IDXGISwapChain3* swapchain;
	RTVDescriptorHeapObject* rtvDescriptorHeap;
	std::vector<ID3D12Resource*> renderTargets;
	unsigned int heapIncrementsize;
	unsigned int bbindex;//back buffer index
public:
	IDXGISwapChain3* GetSwapChain();
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentRTVHeap();
	ID3D12Resource* GetCurrentRenderTarget();
	void SwapChainPresent(UINT SyncInterval, UINT flags);
	void Release();
	SwapChainComponent(HWND& hwnd);
	~SwapChainComponent();
};

