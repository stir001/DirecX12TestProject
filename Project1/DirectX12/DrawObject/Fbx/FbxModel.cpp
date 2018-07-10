#include "FbxModel.h"
#include "TextureObject.h"
#include "Dx12Ctrl.h"
#include "VertexBufferObject.h"
#include "ConstantBufferObject.h"

using namespace Fbx;

FbxModel::FbxModel()
{
}

FbxModel::~FbxModel()
{
}

std::vector<std::shared_ptr<TextureObject>>& Fbx::FbxModel::GetTextureObjects()
{
	return mTextureObjectects;
}
