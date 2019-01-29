#include "CollidableObject.h"
#include <Dx12MSLib.h>

CollidableObject::CollidableObject() : mPos(0.0f, 0.0f, 0.0f), mVel(1.0f, 1.0f, 1.0f)
{
	mPrimitive->SetPosition(mPos);
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
	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_UP))
	{
		mPos.z += mVel.z;
	}
	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_DOWN))
	{
		mPos.z -= mVel.z;
	}
	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_RIGHT))
	{
		mPos.x += mVel.x;
	}
	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_LEFT))
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

