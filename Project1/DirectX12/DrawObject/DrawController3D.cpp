#include "DrawController3D.h"
#include "ConstantBufferObject.h"

using namespace DirectX;

DrawController3D::DrawController3D(const std::string& modelName, const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList): 
	DrawObjectController(modelName +"Bundle", dev, cmdList), mScale(1.0f), mPos(0, 0, 0)
{
	std::string cbufferName = modelName;
	cbufferName += "MatrixBuffer";
	mModelMatrixBuffer.reset(new ConstantBufferObject(cbufferName, dev, sizeof(XMMATRIX), 1));
}

DrawController3D::~DrawController3D()
{
}

void DrawController3D::SetPositon(const DirectX::XMFLOAT3& pos)
{
	mPos = pos;
	UpdateMatrix();
}

void DrawController3D::SetScale(float scale)
{
	mScale = scale;
	UpdateMatrix();
}

void  DrawController3D::AddRotaX(float deg)
{
	DirectX::XMStoreFloat4x4(&mRotationMatrix, XMLoadFloat4x4(&mRotationMatrix) * DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(deg)));
	UpdateMatrix();
}

void  DrawController3D::AddRotaY(float deg)
{
	DirectX::XMStoreFloat4x4(&mRotationMatrix, XMLoadFloat4x4(&mRotationMatrix) * DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(deg)));
	UpdateMatrix();
}

void  DrawController3D::AddRotaZ(float deg)
{
	DirectX::XMStoreFloat4x4(&mRotationMatrix, XMLoadFloat4x4(&mRotationMatrix) * DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(deg)));
	UpdateMatrix();
}

void DrawController3D::SetRotaQuaternion(const DirectX::XMFLOAT4& quaternion)
{
	mQuaternion = quaternion;
	UpdateMatrix();
}

void DrawController3D::UpdateMatrix()
{
	XMMATRIX mat = XMMatrixIdentity();
	XMVECTOR q = XMLoadFloat4(&mQuaternion);
	mat *= XMMatrixRotationQuaternion(q);
	mat *= DirectX::XMLoadFloat4x4(&mRotationMatrix);
	mat *= XMMatrixScaling(mScale, mScale, mScale);
	mat *= XMMatrixTranslation(mPos.x, mPos.y, mPos.z);
	DirectX::XMStoreFloat4x4(&mModelMatrix, mat);

	mModelMatrixBuffer->WriteBuffer256Alignment(&mModelMatrix, sizeof(mModelMatrix), 1);
}

