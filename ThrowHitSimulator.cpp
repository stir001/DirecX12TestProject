#include "ThrowHitSimulator.h"
#include "RandomGenerator.h"
#include <Dx12MSLib.h>


ThrowHitSimulator::ThrowHitSimulator() 
	: mPhase(&ThrowHitSimulator::InitPhase), mCount(0), mVel{ 0.0f, 0.0f, 0.0f }, mVelLength(2.5f)
	, mGroundHitCount(0), mGravity(0.05f)
{
	float radius = 5.0f;
	unsigned int div = 20U;
	mThrowSphere = PrimitiveCreator::Instance().CreateSphere(radius, div);
	mGoalSphere = PrimitiveCreator::Instance().CreateSphere(radius, div);
	float length = 200.0f;
	mGround = PrimitiveCreator::Instance().CreatePlane({ 0,0,0 }, length, length, { 0.0f, 1.0f, 0.0f });
	mGround->SetColor({ 0.6f, 0.3f, 0.0f ,1.0f});
}


ThrowHitSimulator::~ThrowHitSimulator()
{
}

void ThrowHitSimulator::Simulation(const DxInput & input)
{
	(this->*mPhase)();
}

void ThrowHitSimulator::Draw()
{
	mThrowSphere->Draw();
	mGoalSphere->Draw();
	mGround->Draw();
}

void ThrowHitSimulator::InitPhase()
{
	RandomGenerator generator;
	DirectX::XMFLOAT3 range = { 80, 0, 80 };
	mGoalPos = {};
	mGoalPos.x = generator.GetValue(-range.x, range.x);
	mGoalPos.z = generator.GetValue(-range.z, range.z);
	mGoalSphere->SetPosition(mGoalPos);

	mThrowPos = {};
	mThrowSphere->SetPosition(mThrowPos);
	mPhase = &ThrowHitSimulator::ThrowPhase;
	mCount = 0;
}

void ThrowHitSimulator::ThrowPhase()
{
	auto dir = mGoalPos - mThrowPos;
	float distance = GetLengthXMFloat3(dir);
	float sinValue = mGravity * distance / (mVelLength * mVelLength);
	float angle = (3.14159265f / 2.0f) - asinf(sinValue) * 0.5f;
	DirectX::XMFLOAT3 upVec = { 0.0f, 1.0f, 0.0f };
	auto crossVec = NormalizeXMFloat3(CrossXMFloat3(dir, upVec));
	auto quot = ConvertXMMATRIXToXMFloat4x4(DirectX::XMMatrixRotationQuaternion(CreateQuoternion(crossVec, DirectX::XMConvertToDegrees(angle))));
	auto velVec = ConvertXMFloat3ToXMFloat4(NormalizeXMFloat3(dir));
	mVel = ConvertXMFloat4ToXMFloat3(velVec * quot) * mVelLength;
	mGroundHitCount = 0;
	mPhase = &ThrowHitSimulator::RigidPhase;
}

void ThrowHitSimulator::RigidPhase()
{
	mThrowPos += mVel;
	if (mThrowPos.y < 0)
	{
		mThrowPos.y = 0;
		mVel.y = -mVel.y * 0.5f;
		++mGroundHitCount;
		if (mGroundHitCount > 3)
		{
			mPhase = &ThrowHitSimulator::InitPhase;
		}
	}
	mThrowSphere->SetPosition(mThrowPos);
	mVel.y -= mGravity;
}
