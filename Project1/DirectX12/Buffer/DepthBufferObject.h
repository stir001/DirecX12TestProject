#pragma once
#include "Dx12BufferObject.h"

#include <d3d12.h>
#include <wrl.h>

class DepthBufferObject :
	public Dx12BufferObject
{
public:
	DepthBufferObject(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		int height, int width, DXGI_FORMAT format = DXGI_FORMAT_D32_FLOAT);
	DepthBufferObject(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Resource>& resource, D3D12_RESOURCE_STATES state);
	~DepthBufferObject();

	Microsoft::WRL::ComPtr<ID3D12Resource> GetDepthBuffer();
private:
	void Map();
};

