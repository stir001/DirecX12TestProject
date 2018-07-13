#include "Model.h"
#include "Model.h"
#include "Model.h"
#include "Model.h"
#include "Model.h"
#include "IndexBufferObject.h"
#include "VertexBufferObject.h"
#include "ConstantBufferObject.h"
#include "TextureLoader.h"
#include "TextureObject.h"


Model::Model():mTexturecount(0),mIndexBuffer(nullptr),mVertexBuffer(nullptr), mD12mat(nullptr)
{
}

Model::~Model()
{
	mIndexBuffer.reset();
	mVertexBuffer.reset();
	mMaterialBuffer.reset();
	if (mD12mat == nullptr)
	{
		delete mD12mat;
		mD12mat = nullptr;
	}
	for (auto& texObj : mTextureObjects)
	{
		TextureLoader::Instance()->Release(texObj->GetFilePath());
	}
	mTextureObjects.clear();
}

void Model::SetIndexBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList) const
{
	mIndexBuffer->SetBuffer(cmdList);
}

void Model::SetVertexBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList) const
{
	mVertexBuffer->SetBuffer(cmdList);
}

std::shared_ptr<IndexBufferObject> Model::GetIndexBuffer() const
{
	return mIndexBuffer;
}

std::shared_ptr<VertexBufferObject> Model::GetVertexBuffer() const
{
	return mVertexBuffer;
}

std::vector<std::shared_ptr<TextureObject>> Model::GetTextureObjects() const
{
	return mTextureObjects;
}

std::shared_ptr<ConstantBufferObject> Model::GetMaterialBuffer() const
{
	return mMaterialBuffer;
}

const std::string& Model::GetModelName() const
{
	return mModelName;
}

const std::string & Model::GetModelPath() const
{
	return mModelPath;
}
