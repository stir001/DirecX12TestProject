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
	~Dx12CommandList();
	
	const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& GetCommandList() const;
	D3D12_COMMAND_LIST_TYPE GetType() const;

	HRESULT Reset();
	HRESULT Reset(Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState);

	HRESULT SetDescriptorHeap(const std::shared_ptr<Dx12DescriptorHeapObject>& descHeap) const;
	HRESULT TransitionBarrier(std::shared_ptr<Dx12BufferObject>& resource, D3D12_RESOURCE_STATES transitionState) const;

	const std::string& GetName() const;

	void Close() const;
private:
	std::string mName;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCmdList;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mCmdallcator;
	const D3D12_COMMAND_LIST_TYPE mType;
};

