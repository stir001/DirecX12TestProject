#pragma once
#include "DirectX12Buffer.h"


class IndexBufferObject :
	public DirectX12Buffer
{
private:
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
public:
	//indexBuffer,indexBufferview���쐬��MAP������
	IndexBufferObject(unsigned int elementsize, unsigned int elementcount);
	~IndexBufferObject();

	void SetBuffer();
};

