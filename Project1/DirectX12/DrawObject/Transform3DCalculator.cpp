#include "Transform3DCalculator.h"
#include "XMFloatOperators.h"

Transform3DCalculator::Transform3DCalculator():
//, mPos{0,0,0,1}, mScale{1,1,1,1}, mQuaternion(0,0,0,1)
 mAMatrix(StoreMatrixToXMFloat4(DirectX::XMMatrixIdentity()))
//, mRotaMatrix(StoreMatrixToXMFloat4(DirectX::XMMatrixIdentity()))
{
}


Transform3DCalculator::~Transform3DCalculator()
{
}

void Transform3DCalculator::AddPositon(const DirectX::XMFLOAT3& pos)
{
	mAMatrix = Multipli(mAMatrix, DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z));
}

void Transform3DCalculator::AddScale(const DirectX::XMFLOAT3& scale)
{
	mAMatrix = Multipli(mAMatrix, DirectX::XMMatrixScaling(scale.x, scale.y, scale.z));
}

void Transform3DCalculator::AddRotaX(float deg)
{
	mAMatrix = Multipli(mAMatrix, DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(deg)));
}

void Transform3DCalculator::AddRotaY(float deg)
{
	mAMatrix = Multipli(mAMatrix, DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(deg)));
}

void Transform3DCalculator::AddRotaZ(float deg)
{
	mAMatrix = Multipli(mAMatrix, DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(deg)));
}

void Transform3DCalculator::AddRotaQuaternion(const DirectX::XMFLOAT4& quaternion)
{
	DirectX::XMVECTOR q = XMLoadFloat4(&quaternion);
	mAMatrix = Multipli(mAMatrix, DirectX::XMMatrixRotationQuaternion(q));
}

DirectX::XMFLOAT4X4 Transform3DCalculator::GetAMatrix()
{
	return mAMatrix;
}

void Transform3DCalculator::Init()
{
	//mMatrixUpdate = &Transform3DCalculator::UpdateMatrix;
	//mPos = { 0, 0, 0, 1 };
	//mScale = { 1, 1, 1, 1 };
	//mQuaternion = { 0, 0, 0, 1 };
	mAMatrix = StoreMatrixToXMFloat4(DirectX::XMMatrixIdentity());
	//mRotaMatrix = StoreMatrixToXMFloat4(DirectX::XMMatrixIdentity());
}

DirectX::XMFLOAT4X4 Transform3DCalculator::Multipli(DirectX::XMFLOAT4X4 & matF44, DirectX::XMMATRIX & matXM)
{
	DirectX::XMMATRIX mat = DirectX::XMLoadFloat4x4(&matF44);
	mat *= matXM;
	return StoreMatrixToXMFloat4(mat);
}

