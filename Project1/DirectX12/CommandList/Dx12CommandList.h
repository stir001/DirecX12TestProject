#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include <string>

class Dx12BufferObject;
class Dx12DescriptorHeapObject;

class Dx12CommandList
{
public:
	Dx12CommandList(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev , D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);
	virtual ~Dx12CommandList();
	
	const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& GetCommandList() const;
	D3D12_COMMAND_LIST_TYPE GetType() const;

	virtual HRESULT Reset() const;
	virtual HRESULT Reset(Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState);

	HRESULT SetDescriptorHeap(const std::shared_ptr<Dx12DescriptorHeapObject>& descHeap) const;
	HRESULT SetDescriptorHeap(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descHeap) const;
	HRESULT SetGraphicsRootDescriptorTabel(std::shared_ptr<Dx12DescriptorHeapObject>& descHeap, int resourceIndex, int rootpramIndex) const;
	HRESULT SetGraphicsRootDescriptorTable(int rootparamaterIndex, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle) const;
	HRESULT TransitionBarrier(std::shared_ptr<Dx12BufferObject>& resource, D3D12_RESOURCE_STATES transitionState) const;
	
	void ClearDepthView(D3D12_CPU_DESCRIPTOR_HANDLE depthCpuHandle, float clearValue ,D3D12_RECT* rect, unsigned int rectNum = 0);
	void ClearStencilView(D3D12_CPU_DESCRIPTOR_HANDLE stencilCpuHandle, unsigned int clearValue, D3D12_RECT* rect, unsigned int rectNum = 0);
	void OMSetRenderTargets(unsigned int renderTargetCount, D3D12_CPU_DESCRIPTOR_HANDLE rendertargetCpuHandle, D3D12_CPU_DESCRIPTOR_HANDLE* depthCpuHandle = nullptr);
	void ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rendertargetCpuHandle, const float colorRGBA[4], D3D12_RECT* rect, unsigned int rectNum = 0);
	void RSSetViewports(D3D12_VIEWPORT* viewPorts, unsigned int viewportCount = 1);
	void RSSetScissorRects(D3D12_RECT* scissorRects, unsigned int scissorRectsCount = 1);
	const std::string& GetName() const;

	void Close() const;
private:
	std::string mName;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCmdList;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mCmdallcator;
	const D3D12_COMMAND_LIST_TYPE mType;

	HRESULT GetDeviceRemoveReason() const;
};

