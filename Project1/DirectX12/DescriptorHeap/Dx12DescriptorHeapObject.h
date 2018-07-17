#pragma once
#include <wrl.h>
#include <vector>
#include <memory>
#include <d3d12.h>

struct ID3D12DescriptorHeap;
struct ID3D12Device;
class Dx12BufferObject;

class Dx12DescriptorHeapObject
{
public:
	Dx12DescriptorHeapObject(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev, std::vector<std::shared_ptr<Dx12BufferObject>>& buffers, D3D12_DESCRIPTOR_HEAP_TYPE heapType);
	virtual ~Dx12DescriptorHeapObject();

	virtual Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap()const;
	virtual D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHeapHandleStart() const;
	virtual D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHeapHandleStart() const;
	virtual void SetDescriptorHeap(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList) const;
	virtual void SetGprahicsDescriptorTable(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList, unsigned int resourceIndex, unsigned int rootParamaterIndex, unsigned int handleOffsetCount = 0) const;
protected:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDescHeap;
private:
	struct ResourceBind
	{
		std::weak_ptr<Dx12BufferObject> resource;
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	};

	UINT mHeapIncrementSize;
	std::vector<ResourceBind> mResourceBinds;
};

