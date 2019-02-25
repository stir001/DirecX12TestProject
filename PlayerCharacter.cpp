#include "PlayerCharacter.h"
#include <Dx12MSLib.h>

PlayerCharacter::PlayerCharacter(std::shared_ptr<Dx12Camera> camera, float size) 
	: mPos(0.0f, 0.0f, 0.0f), mCamera(camera), mVel(1.0f, 1.0f, 1.0f), mCameraOffset(0.0f, 50.0f, -60.0f)
{
	float coneR = size;
	float coneHeight = coneR * 3.0f;
	unsigned int div = 20U;
	mCone = PrimitiveCreator::Instance().CreateCone(coneR, coneHeight, div);

	float sphereR = coneR * 0.9f;
	mSphere = PrimitiveCreator::Instance().CreateSphere(sphereR, div);
	mSphereOffset = { 0.0f, coneHeight + sphereR, 0.0f };

	mSphere->SetPosition(mSphereOffset);
}


PlayerCharacter::~PlayerCharacter()
{
}

void PlayerCharacter::Draw()
{
	mCone->Draw();
	mSphere->Draw();
}

void PlayerCharacter::Move(const DxInput & input)
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

	SetPosition(mPos);
	mCamera->SetPos(mPos + mCameraOffset);
	mCamera->SetTarget(mPos);

}

void PlayerCharacter::SetPosition(const DirectX::XMFLOAT3 & pos)
{
	mPos = pos;
	mCone->SetPosition(mPos);
	mSphere->SetPosition(mPos + mSphereOffset);
}

void PlayerCharacter::SetCameraOffset(const DirectX::XMFLOAT3 & offset)
{
	mCameraOffset = offset;
}

DirectX::XMFLOAT3 PlayerCharacter::GetPos() const
{
	return mPos;
}
