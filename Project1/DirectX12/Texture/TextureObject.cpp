#include "TextureObject.h"
#include "Dx12Ctrl.h"
#include "RootSignatureObject.h"
#include "ShaderResourceObject.h"

#include <d3d12.h>


TextureObject::TextureObject()
{

}

TextureObject::~TextureObject()
{
	mSubresource.pData = nullptr;
	mShaderResource.reset();
}


int TextureObject::GetWidth() const
{
	return static_cast<int>(mShaderResource->GetBuffer()->GetDesc().Width);
}

int TextureObject::GetHeight() const
{
	return static_cast<int>(mShaderResource->GetBuffer()->GetDesc().Height);
}

std::shared_ptr<ShaderResourceObject> TextureObject::GetShaderResource() const
{
	return mShaderResource;
}

const std::string& TextureObject::GetTextureName() const
{
	return mTextureName;
}

const std::string & TextureObject::GetFilePath() const
{
	return mFilepath;
}

float TextureObject::GetGamma() const
{
	return mGamma;
}
