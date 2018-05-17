#include "FbxModelDataCoverter.h"
#include "FbxModel.h"
#include "IndexBufferObject.h"
#include "VertexBufferObject.h"
#include "Dx12Ctrl.h"

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

	ConvertIndex();
}

void FbxModelDataConverter::ConvertIndex()
{
	mModel->mIndexes = mConvertData->indexes.indexes;

	mModel->indexBuffer = new IndexBufferObject(sizeof(mModel->mIndexes[0]), mModel->mIndexes.size());
	mModel->indexBuffer->WriteBuffer(&mModel->mIndexes[0], mModel->mIndexes.size() * sizeof(mModel->mIndexes[0]));
}

void FbxModelDataConverter::ConvertVertex()
{
	mModel->mVertexes = mConvertData->vertexesInfo.vertexes;

	mModel->vertexBuffer = new VertexBufferObject(sizeof(mModel->mVertexes[0]), mModel->mVertexes.size());
	mModel->vertexBuffer->WriteBuffer256Alignment(&mModel->mVertexes[0], sizeof(mModel->mVertexes[0]) ,mModel->mVertexes.size());
}

void FbxModelDataConverter::ConvertTexture()
{
	DX12CTRL_INSTANCE
	mModel->textureObjects.resize(mConvertData->textures.size());
	int texCount = 0;
	for (auto& texInfo : mConvertData->textures)
	{
		texCount += texInfo.textures.size();
	}
	mModel->textureDescHeap = d12->CreateTextureDescHeap(mConvertData->modelPath, texCount);
	UINT descsize = d12->GetDev()->GetDescriptorHandleIncrementSize(mModel->textureDescHeap->GetDesc().Type);
	for (unsigned int i = 0; i < texCount; ++i)
	{

	}
}
