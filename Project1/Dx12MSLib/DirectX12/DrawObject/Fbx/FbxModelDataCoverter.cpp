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

	for (unsigned int i = static_cast<unsigned int>(size) + 1U; i < static_cast<unsigned int>(data->modelPath.size()); ++i)
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

	mModel.lock()->mIndexBuffer = std::make_shared<IndexBufferObject>(name, dev, sizeof(mModel.lock()->mIndexes[0]), static_cast<unsigned int>(mModel.lock()->mIndexes.size()));
	mModel.lock()->mIndexBuffer->WriteBuffer(&mModel.lock()->mIndexes[0], static_cast<unsigned int>(mModel.lock()->mIndexes.size() * sizeof(mModel.lock()->mIndexes[0])));
}

void FbxModelDataConverter::ConvertVertex(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mModel.lock()->mVertexes = mConvertData.lock()->vertexesInfo.vertexes;
}

void FbxModelDataConverter::ConvertTexture(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	unsigned int texCount = 0;
	for (auto& texSet : mConvertData.lock()->textures)
	{
		texCount += static_cast<unsigned int>(texSet.size());
	}

	mModel.lock()->mTextureObjects.reserve(TEXTURECONT);

	std::wstring wpath;
	wpath.resize(mRelativePath.size());
	for (unsigned int i = 0; i < static_cast<unsigned int>(mRelativePath.size()); ++i)
	{
		wpath[i] = mRelativePath[i];
	}

	int tableIndex = 0;
	for (unsigned int i = 0, j = 0; tableIndex < 3 && i < static_cast<unsigned int>(mConvertData.lock()->textures.size()); ++i)
	{
		j = 0;
		auto& texlayer = mConvertData.lock()->textures[i];
		for (; j < texlayer.size(); ++j)
		{
			if (texlayer[j].texturePath.rfind(TEXTURE_SETABLE[tableIndex])
				<= texlayer[j].texturePath.size())
			{
				break;
			}
		}

		if (texlayer.size() == j)
		{
			continue;
		}

		auto& tex = texlayer[j];
		
		mModel.lock()->mTextureObjects.push_back(TextureLoader::Instance().LoadTexture(mRelativePath + tex.texturePath));

		i = 0;
		++tableIndex;
	}
	
	//足りないテクスチャ分のヌルテクスチャを作成

	std::string convertStr = mConvertData.lock()->modelPath;
	
	convertStr += "NullTexture";

	if (static_cast<unsigned int>(mModel.lock()->mTextureObjects.size()) == 0)
	{
		mModel.lock()->mTextureObjects.push_back(TextureLoader::Instance().LoadTexture(convertStr));
	}

	for (unsigned int i = static_cast<unsigned int>(mModel.lock()->mTextureObjects.size()); i < TEXTURECONT; ++i)
	{
		mModel.lock()->mTextureObjects.push_back(TextureLoader::Instance().LoadTexture(convertStr));
	}
}

void FbxModelDataConverter::ConvertBone(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mModel.lock()->mBones.resize(mConvertData.lock()->bones.size());
	
	for (unsigned int i = 0; i < static_cast<unsigned int>(mModel.lock()->mBones.size()); ++i)
	{
		mModel.lock()->mBones[i] = mConvertData.lock()->bones[i];
	}

	if (static_cast<unsigned int>(mModel.lock()->mBones.size()) == 0)
	{
		Fbx::FbxBone bone;
		DirectX::XMStoreFloat4x4(&bone.initMatrix, DirectX::XMMatrixIdentity());
		mModel.lock()->mBones.push_back(bone);
	}
}

void FbxModelDataConverter::ConvertSkeltons(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{

}
