#include "ImageObject.h"
#include "Dx12Ctrl.h"
#include "VertexBufferObject.h"
#include "TextureObject.h"
#include "XMFloatOperators.h"
#include "Rect.h"

ImageObject::ImageObject(int inwidth, int inheight, std::shared_ptr<TextureObject> intexObj)
	:mWidth(inwidth), mHeight(inheight)
	, mTexObj(intexObj)
{
}

ImageObject::~ImageObject()
{
	mTexObj.reset();
}

DirectX::XMFLOAT2 ImageObject::GetImageSize()
{
	return DirectX::XMFLOAT2(static_cast<float>(mWidth), static_cast<float>(mHeight));
}

std::shared_ptr<ShaderResourceObject> ImageObject::GetShaderResource() const
{
	return mTexObj->GetShaderResource();
}

const std::string& ImageObject::GetTextureName() const
{
	return mTexObj->GetTextureName();
}

const std::string & ImageObject::GetFilePath() const
{
	return mTexObj->GetFilePath();
}
