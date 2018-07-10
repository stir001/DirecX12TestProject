#include "FbxModelDataCoverter.h"
#include "FbxModel.h"
#include "IndexBufferObject.h"
#include "VertexBufferObject.h"
#include "Dx12Ctrl.h"
#include "CharToWChar.h"
#include "FbxModelController.h"

using namespace Fbx;

FbxModelDataConverter::FbxModelDataConverter()
{
}

FbxModelDataConverter::~FbxModelDataConverter()
{
}

FbxModel* FbxModelDataConverter::ConvertToFbxModel(const FbxModelData* data)
{
	mModel = new FbxModel();
	mConvertData = data;
	GetRelativePath(data->modelPath);

	ConvertIndex();
	ConvertVertex();
	//ConvertTexture();
	return mModel;
}

void FbxModelDataConverter::ConvertIndex()
{
	mModel->mIndexes = mConvertData->indexes.indexes;

	mModel->indexBuffer = new IndexBufferObject(sizeof(mModel->mIndexes[0]), static_cast<unsigned int>( mModel->mIndexes.size()));
	mModel->indexBuffer->WriteBuffer(&mModel->mIndexes[0], static_cast<unsigned int>(mModel->mIndexes.size() * sizeof(mModel->mIndexes[0])));
}

void FbxModelDataConverter::ConvertVertex()
{
	mModel->mVertexes = mConvertData->vertexesInfo.vertexes;
	mModel->mVertexElements.resize(mConvertData->vertexesInfo.vertexes.size());
	for (int i = 0; i < mModel->mVertexElements.size(); ++i)
	{
		mModel->mVertexElements[i].pos = mModel->mVertexes[i].pos;
		mModel->mVertexElements[i].normal = mModel->mVertexes[i].normal;
		mModel->mVertexElements[i].texCoord = mModel->mVertexes[i].texCoord;
	}

	size_t size = sizeof(mModel->mVertexElements[0]);
	unsigned int count = static_cast<unsigned int>(mModel->mVertexElements.size());

	mModel->vertexBuffer = new VertexBufferObject(static_cast<unsigned int>(size), count);
	mModel->vertexBuffer->WriteBuffer(&mModel->mVertexElements[0], static_cast<unsigned int>( size * count));
}

void FbxModelDataConverter::ConvertTexture()
{
	DX12CTRL_INSTANCE
	int texCount = 0;
	for (auto& texInfo : mConvertData->textures)
	{
		texCount += static_cast<int>(texInfo.textures.size());
	}
	mModel->textureObjects.resize(texCount);
	mModel->textureDescHeap = d12.CreateTextureDescHeap(mConvertData->modelPath, texCount);
	UINT numdescs = mModel->textureDescHeap->GetDesc().NumDescriptors;
	UINT descsize = d12.GetDev()->GetDescriptorHandleIncrementSize(mModel->textureDescHeap->GetDesc().Type);
	int descOffsetCount = 0;
	for (auto& texLayer : mConvertData->textures)
	{
		for (auto& tex : texLayer.textures)
		{
			std::wstring wpath;
			std::string convertStr = tex.texturePath;
			convertStr.push_back('\0');
			wpath.resize(relativePath.size() + convertStr.size());
			wchar_t* tpath = nullptr;
			ToWChar(&(tpath), convertStr.size(), convertStr.data(), convertStr.size());
			for (int i = 0; i < relativePath.size(); ++i)
			{
				wpath[i] = relativePath[i];
			}
			for (int i = static_cast<int>(relativePath.size()),index = 0; i < wpath.size(); ++i,++index)
			{
				wpath[i] = tpath[index];
			}
			D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = mModel->textureDescHeap->GetCPUDescriptorHandleForHeapStart();
			cpuHandle.ptr += descOffsetCount * descsize;
			D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = mModel->textureDescHeap->GetGPUDescriptorHandleForHeapStart();
			gpuHandle.ptr += descOffsetCount * descsize;
			mModel->textureObjects[descOffsetCount] = d12.LoadTexture(wpath, cpuHandle, gpuHandle);
			++descOffsetCount;

			delete tpath;
		}
	}
}
