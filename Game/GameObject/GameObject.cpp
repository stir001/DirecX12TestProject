#include "GameObject.h"
#include <Dx12MSLib.h>


GameObject::GameObject() : mCtrl(nullptr), mData(nullptr), mPos{0.0f, 0.0f, 0.0f}, mVel(0.0f, 0.0f, 0.0f)
{
}

GameObject::GameObject(const std::shared_ptr<PrimitiveController>& ctrl, const std::shared_ptr<PrimitiveObject>& data)
	: mCtrl(ctrl), mData(data), mPos{ 0.0f,0.0f,0.0f }, mVel(0.0f, 0.0f, 0.0f)
{
	SetCollisionRadius();
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	mPos += mVel;
	mCtrl->SetPosition(mPos);
}

void GameObject::SetVelocity(const DirectX::XMFLOAT3 & vel)
{
	mVel = vel;
}

DirectX::XMFLOAT3 GameObject::GetVelocity() const
{
	return mVel;
}

void GameObject::SetPos(const DirectX::XMFLOAT3 & pos)
{
	mPos = pos;
	mCtrl->SetPosition(mPos);
}

DirectX::XMFLOAT3 GameObject::GetPos() const
{
	return mPos;
}

void GameObject::Draw() const
{
	mCtrl->Draw();
}

std::shared_ptr<PrimitiveObject>& GameObject::GetData()
{
	return mData;
}

const std::vector<PrimitiveVertex>& GameObject::GetVertices() const
{
	return mData->GetVertices();
}

const std::vector<unsigned int>& GameObject::GetIndices() const
{
	return mData->GetIndices();
}

void GameObject::Reset(const std::shared_ptr<PrimitiveController>& ctrl, const std::shared_ptr<PrimitiveObject>& data)
{
	mCtrl = ctrl;
	mData = data;
	SetCollisionRadius();
}

void GameObject::SetColor(const DirectX::XMFLOAT3& color)
{
	mCtrl->SetColor({ color.x, color.y, color.z, 1.0f });
}

float GameObject::GetCollisionRadius() const
{
	return mCollisionRadius;
}

void GameObject::SetCollisionRadius()
{
	DirectX::XMFLOAT3 max = { FLT_MIN, FLT_MIN, FLT_MIN };
	DirectX::XMFLOAT3 min = { FLT_MAX, FLT_MAX, FLT_MAX };

	auto verts = mData->GetVertices();

	for (auto& v : verts)
	{
		max.x = v.pos.x > max.x ? v.pos.x : max.x;
		max.y = v.pos.y > max.y ? v.pos.y : max.y;
		max.z = v.pos.z > max.z ? v.pos.z : max.z;

		min.x = v.pos.x < min.x ? v.pos.x : min.x;
		min.y = v.pos.y < min.y ? v.pos.y : min.y;
		min.z = v.pos.z < min.z ? v.pos.z : min.z;
	}

	auto len = GetLength(max - min);

	mCollisionRadius = len * 0.5f;
}
