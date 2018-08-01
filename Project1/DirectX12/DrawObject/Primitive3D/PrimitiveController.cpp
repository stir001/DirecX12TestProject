#include "PrimitiveController.h"
#include "PrimitiveObject.h"
#include "ConstantBufferObject.h"
#include "Dx12Ctrl.h"


PrimitiveController::PrimitiveController(std::shared_ptr<PrimitiveObject> primitive
	, Microsoft::WRL::ComPtr<ID3D12Device>& dev
	, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
	: DrawController3D(primitive->GetName(),dev, cmdList)
	, mPrimitive(primitive)
{
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

	mInstanceMatrixBuffer.reset(new ConstantBufferObject(mPrimitive->GetName() + "ConstantBuffer", Dx12Ctrl::Instance().GetDev(), sizeof(mInstancesMatrix[0]), mInstancesMatrix.size()));

}
