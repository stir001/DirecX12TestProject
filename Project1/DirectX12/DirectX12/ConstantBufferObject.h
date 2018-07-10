#pragma once
#include "Dx12BufferObject.h"
#include <functional>

class ConstantBufferObject :
	public Dx12BufferObject
{
public:
	ConstantBufferObject(const std::string& name,const Microsoft::WRL::ComPtr<ID3D12Device>& dev, 
		unsigned int elementsize, unsigned int elementcount);
	ConstantBufferObject(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Resource>& rsc,
		D3D12_RESOURCE_STATES state, unsigned int elementsize, unsigned int elementcount);
	~ConstantBufferObject();

	void UpdateBuffer();
	
protected:
	void SetUpdate(std::function<void(void)>& updataFunc);

private:
	std::function<void(void)> mSetBufferUpdate;
};

