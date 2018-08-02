#include "PrimitiveController.h"
#include "PrimitiveObject.h"
#include "ConstantBufferObject.h"
#include "VertexBufferObject.h"
#include "IndexBufferObject.h"
#include "Dx12DescriptorHeapObject.h"
#include "Dx12Ctrl.h"


PrimitiveController::PrimitiveController(std::shared_ptr<PrimitiveObject> primitive
	, Microsoft::WRL::ComPtr<ID3D12Device>& dev
	, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
	: DrawController3D(primitive->GetName(),dev, cmdList)
	, mPrimitive(primitive), mUpdate(&PrimitiveController::UpdateBuffer)
{
	mVertexBuffer.reset(new VertexBufferObject(primitive->GetName() + "VertexBuffer", dev, sizeof(PrimitiveVertex), primitive->GetVertices().size()));
	mIndexBuffer.reset(new IndexBufferObject(primitive->GetName() + "IndexBuffer", dev, sizeof(unsigned int) ,primitive->GetIndices().size()));
	std::vector<DirectX::XMFLOAT3> pos(1);
	pos[0] = DirectX::XMFLOAT3(0, 0, 0);
	Instancing(pos);
}


PrimitiveController::~PrimitiveController()
{
}

void PrimitiveController::Instancing(std::vector<DirectX::XMFLOAT3>& instancePositions)
{
	mInstancesMatrix.clear();
	mInstancesMatrix.resize(instancePositions.size());
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMStoreFloat4x4(&mat, DirectX::XMMatrixIdentity());
	for (int i = 0; i < instancePositions.size(); ++i)
	{
		mat._41 = instancePositions[i].x;
		mat._42 = instancePositions[i].y;
		mat._43 = instancePositions[i].z;
		mInstancesMatrix[i] = mat;
	}

	mInstanceMatrixBuffer.reset(new VertexBufferObject(mPrimitive->GetName() + "InstanceVertexBuffer", Dx12Ctrl::Instance().GetDev(), sizeof(mInstancesMatrix[0]), mInstancesMatrix.size()));

	mUpdate = &PrimitiveController::UpdateBuffer;
}

void PrimitiveController::Draw()
{
	(this->*mUpdate)();
	mIndexBuffer->SetBuffer(mCmdList);
	mVertexBuffer;
	mDescHeap->SetDescriptorHeap(mCmdList);
}

void PrimitiveController::UpdateBuffer()
{
	mInstanceMatrixBuffer->WriteBuffer256Alignment(mInstancesMatrix.data(), sizeof(mInstancesMatrix[0]), mInstancesMatrix.size());
	mUpdate = &PrimitiveController::NonUpdateBuffer;
}

void PrimitiveController::NonUpdateBuffer()
{
}
