#include "PlayerCharacter.h"
#include <Dx12MSLib.h>

PlayerCharacter::PlayerCharacter(std::shared_ptr<Dx12Camera> camera) 
	: mPos(0.0f, 0.0f, 0.0f), mCamera(camera), mVel(1.0f, 1.0f, 1.0f), mCameraOffset(0.0f, 30.0f, -60.0f)
{
	float coneR = 5.0f;
	float coneHeight = 15.0f;
	unsigned int div = 20U;
	mCone = PrimitiveCreator::Instance().CreateCone(coneR, coneHeight, div);

	float sphereR = 4.0f;
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
	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_W))
	{
		mPos.z += mVel.z;
	}

	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_S))
	{
		mPos.z -= mVel.z;
	}

	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_D))
	{
		mPos.x += mPos.x;
	}
	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_A))
	{
		mPos.x -= mPos.x;
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
