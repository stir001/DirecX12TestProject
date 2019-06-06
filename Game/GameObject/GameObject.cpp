#include "GameObject.h"
#include <Dx12MSLib.h>


GameObject::GameObject() : mCtrl(nullptr), mData(nullptr), mPos{0.0f, 0.0f, 0.0f}, mVel(0.0f, 0.0f, 0.0f)
{
}

GameObject::GameObject(const std::shared_ptr<PrimitiveController>& ctrl, const std::shared_ptr<PrimitiveObject>& data)
	: mCtrl(ctrl), mData(data), mPos{ 0.0f,0.0f,0.0f }, mVel(0.0f, 0.0f, 0.0f)
{
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
}
