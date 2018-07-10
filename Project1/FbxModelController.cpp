#include "FbxModelController.h"
#include "FbxModel.h"
#include "Dx12Ctrl.h"
#include "DirectionalLight.h"
#include "ConstantBufferObject.h"

using namespace Fbx;
using namespace DirectX;

FbxModelController::FbxModelController(): mModelMatrix(DirectX::XMMatrixIdentity()), mScale(1.0f), mPos(0,0,0),mRotationMatrix(XMMatrixIdentity())
{
	XMStoreFloat4(&mQuaternion, XMQuaternionIdentity());
	mModelMatrixBuffer = new ConstantBufferObject(sizeof(XMMATRIX), 1);
	mModelMatrixBuffer->SetRootparameterIndex(3);
	DX12CTRL_INSTANCE

	mModelMatrixBuffer->SetCommandList(d12.GetCmdList().Get());
	UpdateMatrix();
}

FbxModelController::FbxModelController(std::shared_ptr<FbxModel> model): mModelMatrix(DirectX::XMMatrixIdentity()), mScale(1.0f), mPos(0,0,0),mModel(model), mRotationMatrix(XMMatrixIdentity())
{
	DX12CTRL_INSTANCE
	XMStoreFloat4(&mQuaternion, XMQuaternionIdentity());
	mModelMatrixBuffer = new ConstantBufferObject(sizeof(XMMATRIX), 1);
	mModelMatrixBuffer->SetRootparameterIndex(3);
	mModelMatrixBuffer->SetCommandList(d12.GetCmdList().Get());
	UpdateMatrix();
}

FbxModelController::~FbxModelController()
{
}

void FbxModelController::Draw()
{
	DX12CTRL_INSTANCE
	d12.GetCmdList()->SetPipelineState(d12.GetPiplineState(pos_fbx).Get());
	d12.GetCmdList()->SetGraphicsRootSignature(d12.GetRootSignature(rsi_fbx).Get());
	d12.SetCameraBuffer();
	mDirLight->SetLight();
	mModel->SetIndexBuffer();
	mModel->SetVertexBuffer();
	//mModel->SetTexture();
	mModelMatrixBuffer->SetDescHeap();
	mModelMatrixBuffer->SetBuffer();
	d12.GetCmdList()->DrawIndexedInstanced(static_cast<UINT>(mModel->mIndexes.size()), 1, 0, 0, 0);
}

void FbxModelController::SetLight(std::shared_ptr<DirectionalLight> dirlight)
{
	mDirLight = dirlight;
}

void FbxModelController::SetPositon(DirectX::XMFLOAT3& pos)
{
	mPos = pos;
	UpdateMatrix();
}

void FbxModelController::SetScale(float scale)
{
	mScale = scale;
	UpdateMatrix();
}

void  FbxModelController::AddRotaX(float deg)
{
	mRotationMatrix = XMMatrixRotationX(XMConvertToRadians(deg)) * mRotationMatrix;
	UpdateMatrix();
}

void  FbxModelController::AddRotaY(float deg)
{
	mRotationMatrix = XMMatrixRotationY(XMConvertToRadians(deg)) * mRotationMatrix;
	UpdateMatrix();
}

void  FbxModelController::AddRotaZ(float deg)
{
	mRotationMatrix = XMMatrixRotationZ(XMConvertToRadians(deg)) * mRotationMatrix;
	UpdateMatrix();
}

void FbxModelController::SetRotaQuaternion(DirectX::XMFLOAT4& quaternion)
{
	mQuaternion = quaternion;
	UpdateMatrix();
}

void FbxModelController::UpdateMatrix()
{
	mModelMatrix = XMMatrixIdentity();
	mModelMatrix *= XMMatrixTranslation(mPos.x, mPos.y, mPos.z);
	XMVECTOR q = XMLoadFloat4(&mQuaternion);
	mModelMatrix *= XMMatrixRotationQuaternion(q);
	mModelMatrix *= mRotationMatrix;
	mModelMatrix *= XMMatrixScaling(mScale, mScale, mScale);

	mModelMatrixBuffer->WriteBuffer256Alignment(&mModelMatrix, sizeof(mModelMatrix), 1);
}
