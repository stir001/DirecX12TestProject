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
	for (auto& texObj : mTextureObjectects)
	{
		TextureLoader::Instance()->Release(texObj->GetFilePath());
	}
	mTextureObjectects.clear();
}

void Model::SetIndexBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList) const
{
	mIndexBuffer->SetBuffer(cmdList);
}

void Model::SetVertexBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList) const
{
	mVertexBuffer->SetBuffer(cmdList);
}

const std::string& Model::GetModelName() const
{
	return mModelName;
}

const std::string & Model::GetModelPath() const
{
	return mModelPath;
}
