#include "ImageObject.h"
#include "Dx12Ctrl.h"
#include "VertexBufferObject.h"
#include "TextureObj.h"
#include "XMFloat3Operators.h"
#include "Rect.h"

ImageObject::ImageObject(int inwidth, int inheight, TextureObj* intexObj, ID3D12DescriptorHeap* intexDescHeap) 
	:mWidth(inwidth), mHeight(inheight)
	, mTexObj(intexObj), mTexDescHeap(intexDescHeap)
{

}

ImageObject::~ImageObject()
{
}

DirectX::XMFLOAT2 ImageObject::GetImageSize()
{
	return DirectX::XMFLOAT2(mWidth, mHeight);
}

void ImageObject::SetImage()
{
	DX12CTRL_INSTANCE
	d12->GetCmdList()->SetDescriptorHeaps(1, &mTexDescHeap);
	mTexObj->SetBuffer();
}
