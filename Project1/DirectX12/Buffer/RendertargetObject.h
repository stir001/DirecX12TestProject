#pragma once
#include "Dx12BufferObject.h"

class RendertargetObject : public Dx12BufferObject
{
public:
	RendertargetObject(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Device>& dev, 
		unsigned int width, unsigned int height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
	RendertargetObject(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Resource>& rsc);
	~RendertargetObject();

	void SetClearValue(D3D12_CLEAR_VALUE clrValue);
	D3D12_CLEAR_VALUE GetClearValue() const;
private:
	D3D12_CLEAR_VALUE mClearValue;
};

