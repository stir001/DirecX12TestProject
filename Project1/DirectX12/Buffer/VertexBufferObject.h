#pragma once
#include "Dx12BufferObject.h"
class VertexBufferObject :
	public Dx12BufferObject
{
public:
	VertexBufferObject(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev, 
		unsigned int elementsize, unsigned int elementcount);
	VertexBufferObject(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Resource>& rsc,
		unsigned int elementSize, unsigned int elementCount);
	~VertexBufferObject();

	void SetBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList) const;
	const D3D12_VERTEX_BUFFER_VIEW* GetViewPtr() const;
private:
	D3D12_VERTEX_BUFFER_VIEW mVbView;
};

