#pragma once
#include "Dx12BufferObject.h"
class ShaderResourceObject :
	public Dx12BufferObject
{
public:
	ShaderResourceObject(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev, unsigned int width, unsigned int height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
	ShaderResourceObject(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Resource>& rsc);
	~ShaderResourceObject();

private:
	D3D12_CLEAR_VALUE mClearValue;
};

