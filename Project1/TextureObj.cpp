#include "TextureObj.h"
#include "Dx12Ctrl.h"
#include "RootSignatureObject.h"

#include <d3d12.h>


TextureObj::TextureObj() :textureBuffer(nullptr)//, texSRVDescHeap(nullptr)
{
	ZeroMemory(&subresource, sizeof(subresource));
}

TextureObj::~TextureObj()
{
	if(textureBuffer != nullptr)
	textureBuffer->Release();
	decodedData.release();
	subresource.pData = nullptr;
}

void TextureObj::SetBuffer()
{
	DX12CTRL_INSTANCE
	
	d12->GetCmdList()->SetGraphicsRootDescriptorTable(rpt_texture, gpuHandle);
}

//ID3D12DescriptorHeap* TextureObj::GetDescHeap()
//{
//	return texSRVDescHeap;
//}

int TextureObj::GetWidth()
{
	return static_cast<int>(textureBuffer->GetDesc().Width);
}

int TextureObj::GetHeight()
{
	return static_cast<int>(textureBuffer->GetDesc().Height);
}
