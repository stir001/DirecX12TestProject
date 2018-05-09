#pragma once
#include "DirectX12Buffer.h"
class VertexBufferObject :
	public DirectX12Buffer
{
private:
	D3D12_VERTEX_BUFFER_VIEW vbView;
	void* vertexElementBuffer;
public:
	void SetBuffer() const;
	VertexBufferObject(unsigned int elementsize, unsigned int elementcount);
	~VertexBufferObject();
};

