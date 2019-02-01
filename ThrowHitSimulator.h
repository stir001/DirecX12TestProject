#pragma once
#include <memory>
#include <DirectXMath.h>

class DxInput;
class PrimitiveController;

class ThrowHitSimulator
{
public:
	ThrowHitSimulator();
	~ThrowHitSimulator();
	void Simulation(const DxInput& input);
	void Draw();
private:
	void InitPhase();
	void ThrowPhase();
	void RigidPhase();
	
	void (ThrowHitSimulator::*mPhase)();

	std::shared_ptr<PrimitiveController> mThrowSphere;
	std::shared_ptr<PrimitiveController> mGoalSphere;
	std::shared_ptr<PrimitiveController> mGround;
	DirectX::XMFLOAT3 mGoalPos;
	DirectX::XMFLOAT3 mThrowPos;
	DirectX::XMFLOAT3 mVel;
	float mVelLength;
	int mGroundHitCount;
	float mGravity;
	unsigned int mCount;
};

