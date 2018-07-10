#include "TextureObj.h"
#include "Dx12Ctrl.h"
#include "RootSignatureObject.h"

#include <d3d12.h>


TextureObj::TextureObj() :textureBuffer(nullptr)
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

void TextureObj::SetBuffer() const
{
	DX12CTRL_INSTANCE
	d12.GetCmdList()->SetGraphicsRootDescriptorTable(rpt_texture, gpuHandle);
}

int TextureObj::GetWidth() const
{
	return static_cast<int>(textureBuffer->GetDesc().Width);
}

int TextureObj::GetHeight() const
{
	return static_cast<int>(textureBuffer->GetDesc().Height);
}

void TextureObj::SetBuffer(int rootparamIndex) const
{
	DX12CTRL_INSTANCE
	d12.GetCmdList()->SetGraphicsRootDescriptorTable(rootparamIndex, gpuHandle);
}
