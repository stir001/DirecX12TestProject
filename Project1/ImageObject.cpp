#include "ImageObject.h"
#include "Dx12Ctrl.h"
#include "VertexBufferObject.h"
#include "TextureObj.h"
#include "XMFloat3Operators.h"
#include "Rect.h"
#include "TextureObj.h"

ImageObject::ImageObject(int inwidth, int inheight, TextureObj* intexObj, ID3D12DescriptorHeap* intexDescHeap) 
	:mWidth(inwidth), mHeight(inheight)
	, mTexObj(intexObj), mTexDescHeap(intexDescHeap),mGamma(intexObj->GetGamma())
{

}

ImageObject::~ImageObject()
{
}

DirectX::XMFLOAT2 ImageObject::GetImageSize()
{
	return DirectX::XMFLOAT2(static_cast<float>(mWidth), static_cast<float>(mHeight));
}

void ImageObject::SetImage() const
{
	DX12CTRL_INSTANCE
	d12->GetCmdList()->SetDescriptorHeaps(1, &mTexDescHeap);
	mTexObj->SetBuffer();
}

float ImageObject::GetGamma()const
{
	return mGamma;
}
