#include "CollidableObject.h"
#include <Dx12MSLib.h>

CollidableObject::CollidableObject() : mPos(0.0f, 0.0f, 0.0f), mVel(0.5f, 0.5f, 0.5f)
{

}

CollidableObject::~CollidableObject()
{
}

void CollidableObject::Draw()
{
	mPrimitive->Draw();
}

void CollidableObject::DefaultMove(const DxInput & input)
{
	if (input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_W))
	{
		mPos.z += mVel.z;
	}
	if (input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_S))
	{
		mPos.z -= mVel.z;
	}
	if (input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_D))
	{
		mPos.x += mVel.x;
	}
	if (input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_A))
	{
		mPos.x -= mVel.x;
	}
	mPrimitive->SetPosition(mPos);
}

void CollidableObject::SetPos(const DirectX::XMFLOAT3 & pos)
{
	mPos = pos;
	mPrimitive->SetPosition(pos);
}

void CollidableObject::AddPos(const DirectX::XMFLOAT3 & offset)
{
	mPos += offset;
	mPrimitive->SetPosition(mPos);
}

void CollidableObject::SetVel(const DirectX::XMFLOAT3 & vel)
{
	mVel = vel;
}

void CollidableObject::AddRotateY(float deg)
{
	mPrimitive->AddRotaY(deg);
}

void CollidableObject::AddRotateX(float deg)
{
	mPrimitive->AddRotaX(deg);
}

void CollidableObject::AddRotateZ(float deg)
{
	mPrimitive->AddRotaZ(deg);
}

void CollidableObject::SetColor(const DirectX::XMFLOAT3 & color)
{
	mPrimitive->SetColor(ConvertXMFloat3ToXMFloat4(color));
}

DirectX::XMFLOAT3 CollidableObject::GetPos() const
{
	return mPos;
}

