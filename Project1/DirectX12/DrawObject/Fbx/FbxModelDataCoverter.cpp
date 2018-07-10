#include "FbxModelDataCoverter.h"
#include "FbxModel.h"
#include "IndexBufferObject.h"
#include "VertexBufferObject.h"
#include "Dx12Ctrl.h"
#include "CharToWChar.h"
#include "FbxModelController.h"
#include "ConstantBufferObject.h"
#include "XMFloatOperators.h"
#include "Util.h"
#include "TextureLoader.h"

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

FbxModel* FbxModelDataConverter::ConvertToFbxModel(const FbxModelData* data)
{
	mModel = new FbxModel();
	mModel->mModelPath = data->modelPath;
	mConvertData = data;
	mRelativePath = GetRelativePath(data->modelPath);

	size_t size = data->modelPath.rfind('/');

	for (unsigned int i = size + 1; i < data->modelPath.size(); ++i)
	{
		mModel->mModelName.push_back(data->modelPath[i]);
	}

	auto& dev = Dx12Ctrl::Instance()->GetDev();

	ConvertIndex(dev);
	ConvertVertex(dev);
	ConvertTexture(dev);
	ConvertBone(dev);
	return mModel;
}

void FbxModelDataConverter::ConvertIndex(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mModel->mIndexes = mConvertData->indexes.indexes;
	std::string name = mModel->GetModelName();
	name += "IndexBuffer";

	mModel->mIndexBuffer.reset(new IndexBufferObject(name,dev,sizeof(mModel->mIndexes[0]), static_cast<unsigned int>( mModel->mIndexes.size())));
	mModel->mIndexBuffer->WriteBuffer(&mModel->mIndexes[0], static_cast<unsigned int>(mModel->mIndexes.size() * sizeof(mModel->mIndexes[0])));
}

void FbxModelDataConverter::ConvertVertex(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mModel->mVertexes = mConvertData->vertexesInfo.vertexes;
}

void FbxModelDataConverter::ConvertTexture(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	unsigned int texCount = 0;
	for (auto& texInfo : mConvertData->textures)
	{
		texCount += texInfo.textures.size();
	}

	mModel->mTextureObjectects.reserve(TEXTURECONT);

	std::wstring wpath;
	wpath.resize(mRelativePath.size());
	for (unsigned int i = 0; i < mRelativePath.size(); ++i)
	{
		wpath[i] = mRelativePath[i];
	}

	int tableIndex = 0;
	for (unsigned int i = 0, j = 0; tableIndex < 3 && i < mConvertData->textures.size(); ++i)
	{
		j = 0;
		auto& texlayer = mConvertData->textures[i];
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
		
		mModel->mTextureObjectects.push_back(TextureLoader::Instance()->LoadTexture(mRelativePath + tex.texturePath));


		i = 0;
		++tableIndex;
	}
	
	//足りないテクスチャ分のヌルテクスチャを作成

	std::string convertStr = mConvertData->modelPath;
	
	convertStr += "NullTexture";

	if (mModel->mTextureObjectects.size() == 0)
	{
		mModel->mTextureObjectects.push_back(TextureLoader::Instance()->LoadTexture(convertStr));
	}

	for (unsigned int i = mModel->mTextureObjectects.size(); i < TEXTURECONT; ++i)
	{
		mModel->mTextureObjectects.push_back(TextureLoader::Instance()->LoadTexture(convertStr));
	}
}

void FbxModelDataConverter::ConvertBone(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mModel->mBones.resize(mConvertData->bones.size());
	
	for (unsigned int i = 0; i < mModel->mBones.size(); ++i)
	{
		mModel->mBones[i] = mConvertData->bones[i];
	}

	if (mModel->mBones.size() == 0)
	{
		Fbx::FbxBone bone;
		DirectX::XMStoreFloat4x4(&bone.initMatrix, DirectX::XMMatrixIdentity());
		mModel->mBones.push_back(bone);
	}
}
