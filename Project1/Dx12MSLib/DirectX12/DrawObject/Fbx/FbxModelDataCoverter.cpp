#include "stdafx.h"
#include "FbxModelDataCoverter.h"
#include "FbxModel.h"
#include "FbxModelController.h"
#include "Buffer/IndexBufferObject.h"
#include "Buffer/VertexBufferObject.h"
#include "Buffer/ConstantBufferObject.h"
#include "Master/Dx12Ctrl.h"
#include "Util/CharToWChar.h"
#include "Util/XMFloatOperators.h"
#include "Util/Util.h"
#include "Texture/TextureLoader.h"

using namespace Fbx;

const unsigned int TEXTURECONT = 3;

const std::string TEXTURE_SETABLE[TEXTURECONT] =
{
	{"_d.png"},
	{"_s.png"},
	{"_b.png"},
};

FbxModelDataConverter::FbxModelDataConverter()
{
}

FbxModelDataConverter::~FbxModelDataConverter()
{
}

std::shared_ptr<FbxModel> FbxModelDataConverter::ConvertToFbxModel(std::shared_ptr<FbxModelData> data)
{
	std::shared_ptr<FbxModel> model = std::make_shared<FbxModel>();
	mModel = model;
	mModel.lock()->mModelPath = data->modelPath;
	mConvertData = data;
	mRelativePath = GetRelativePath(data->modelPath);

	size_t size = data->modelPath.rfind('/');

	for (unsigned int i = size + 1; i < static_cast<int>(data->modelPath.size()); ++i)
	{
		mModel.lock()->mModelName.push_back(data->modelPath[i]);
	}

	auto& dev = Dx12Ctrl::Instance().GetDev();

	ConvertIndex(dev);
	ConvertVertex(dev);
	ConvertTexture(dev);
	ConvertBone(dev);
	return model;
}

void FbxModelDataConverter::ConvertIndex(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mModel.lock()->mIndexes = mConvertData.lock()->indexes.indexes;
	std::string name = mModel.lock()->GetModelName();
	name += "IndexBuffer";

	mModel.lock()->mIndexBuffer.reset(new IndexBufferObject(name,dev,sizeof(mModel.lock()->mIndexes[0]), static_cast<unsigned int>( mModel.lock()->mIndexes.size())));
	mModel.lock()->mIndexBuffer->WriteBuffer(&mModel.lock()->mIndexes[0], static_cast<unsigned int>(mModel.lock()->mIndexes.size() * sizeof(mModel.lock()->mIndexes[0])));
}

void FbxModelDataConverter::ConvertVertex(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mModel.lock()->mVertexes = mConvertData.lock()->vertexesInfo.vertexes;
}

void FbxModelDataConverter::ConvertTexture(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	unsigned int texCount = 0;
	for (auto& texInfo : mConvertData.lock()->textures)
	{
		texCount += static_cast<int>(texInfo.textures.size());
	}

	mModel.lock()->mTextureObjects.reserve(TEXTURECONT);

	std::wstring wpath;
	wpath.resize(mRelativePath.size());
	for (unsigned int i = 0; i < mRelativePath.size(); ++i)
	{
		wpath[i] = mRelativePath[i];
	}

	int tableIndex = 0;
	for (unsigned int i = 0, j = 0; tableIndex < 3 && i < mConvertData.lock()->textures.size(); ++i)
	{
		j = 0;
		auto& texlayer = mConvertData.lock()->textures[i];
		for (; j < texlayer.textures.size(); ++j)
		{
			if (texlayer.textures[j].texturePath.rfind(TEXTURE_SETABLE[tableIndex])
				<= texlayer.textures[j].texturePath.size())
			{
				break;
			}
		}

		if (texlayer.textures.size() == j)
		{
			continue;
		}

		auto& tex = texlayer.textures[j];
		
		mModel.lock()->mTextureObjects.push_back(TextureLoader::Instance().LoadTexture(mRelativePath + tex.texturePath));


		i = 0;
		++tableIndex;
	}
	
	//足りないテクスチャ分のヌルテクスチャを作成

	std::string convertStr = mConvertData.lock()->modelPath;
	
	convertStr += "NullTexture";

	if (mModel.lock()->mTextureObjects.size() == 0)
	{
		mModel.lock()->mTextureObjects.push_back(TextureLoader::Instance().LoadTexture(convertStr));
	}

	for (unsigned int i = static_cast<int>(mModel.lock()->mTextureObjects.size()); i < TEXTURECONT; ++i)
	{
		mModel.lock()->mTextureObjects.push_back(TextureLoader::Instance().LoadTexture(convertStr));
	}
}

void FbxModelDataConverter::ConvertBone(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mModel.lock()->mBones.resize(mConvertData.lock()->bones.size());
	
	for (unsigned int i = 0; i < mModel.lock()->mBones.size(); ++i)
	{
		mModel.lock()->mBones[i] = mConvertData.lock()->bones[i];
	}

	if (mModel.lock()->mBones.size() == 0)
	{
		Fbx::FbxBone bone;
		DirectX::XMStoreFloat4x4(&bone.initMatrix, DirectX::XMMatrixIdentity());
		mModel.lock()->mBones.push_back(bone);
	}
}
