#pragma once
#include "Dx12BufferObject.h"


class IndexBufferObject :
	public Dx12BufferObject
{
private:
	D3D12_INDEX_BUFFER_VIEW mIndexBufferView;
public:
	//indexBuffer,indexBufferviewÇçÏê¨ÇµMAPÇÇ∑ÇÈ
	IndexBufferObject(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev, 
		unsigned int elementsize, unsigned int elementcount, DXGI_FORMAT format = DXGI_FORMAT_R32_UINT);
	IndexBufferObject(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Resource>& rsc);
	~IndexBufferObject();

	void SetBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList) const;
};

