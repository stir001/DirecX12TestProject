#include "Model.h"
#include "IndexBufferObject.h"
#include "VertexBufferObject.h"
#include "ConstantBufferObject.h"


Model::Model():texturecount(0),indexBuffer(nullptr),vertexBuffer(nullptr)
{
}


Model::~Model()
{
	delete indexBuffer;
	delete vertexBuffer;
	delete materialBuffer;
	delete d12mat;
}

void Model::SetIndexBuffer()
{
	indexBuffer->SetBuffer();
}

void Model::SetVertexBuffer()
{
	vertexBuffer->SetBuffer();
}

void Model::SetMaterialBuffer()
{
	materialBuffer->SetDescHeap();
}