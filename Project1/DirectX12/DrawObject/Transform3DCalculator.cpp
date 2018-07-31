#include "Transform3DCalculator.h"
#include "XMFloatOperators.h"

Transform3DCalculator::Transform3DCalculator():mMatrixUpdate(&Transform3DCalculator::UpdateMatrix)
, mPos{0,0,0,1}, mScale{1,1,1,1}, mQuaternion(0,0,0,1)
, mAMatrix(StoreMatirxToXMFloat4(DirectX::XMMatrixIdentity()))
, mRotaMatrix(StoreMatirxToXMFloat4(DirectX::XMMatrixIdentity()))
{
}


Transform3DCalculator::~Transform3DCalculator()
{
}

void Transform3DCalculator::SetPositon(const DirectX::XMFLOAT3& pos)
{
	mPos = StoreFloat3ToXMFloat4(pos);
}

void Transform3DCalculator::SetScale(DirectX::XMFLOAT3& scale)
{
	mScale = StoreFloat3ToXMFloat4(scale);
}

void Transform3DCalculator::AddRotaX(float deg)
{
	 DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(deg));
}

void Transform3DCalculator::AddRotaY(float deg)
{
	DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(deg));
}

void Transform3DCalculator::AddRotaZ(float deg)
{
	DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(deg));
}

void Transform3DCalculator::SetRotaQuaternion(const DirectX::XMFLOAT4& quaternion)
{
	mQuaternion = quaternion;
}

DirectX::XMFLOAT4X4 Transform3DCalculator::GetAMatrix()
{
	(this->*mMatrixUpdate)();
	return mAMatrix;
}

void Transform3DCalculator::UpdateMatrix()
{
	DirectX::XMMATRIX mat = DirectX::XMMatrixIdentity();
	DirectX::XMVECTOR q = XMLoadFloat4(&mQuaternion);
	mat *= DirectX::XMMatrixRotationQuaternion(q);
	mat *= DirectX::XMLoadFloat4x4(&mRotaMatrix);
	mat *= DirectX::XMMatrixScaling(mScale.x, mScale.y, mScale.z);
	mat *= DirectX::XMMatrixTranslation(mPos.x, mPos.y, mPos.z);
	mAMatrix = StoreMatirxToXMFloat4(mat);

	mMatrixUpdate = &Transform3DCalculator::NonUpdateMatrix;
}

void Transform3DCalculator::NonUpdateMatrix()
{
}
