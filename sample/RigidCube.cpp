#include "RigidCube.h"
#include "Dx12MSLib.h"
#include "BulletInclude.h"

#include <random>

RigidCube::RigidCube()
{
	std::random_device seed;
	std::mt19937 engine(seed());
	std::uniform_real_distribution<float> xzRandom(-7, 7);
	std::uniform_real_distribution<float> yRandom(20, 27);
	DirectX::XMFLOAT3 pos = { xzRandom(engine)
	, yRandom(engine)
	, xzRandom(engine) };

	mCubeCtrl = PrimitiveCreator::Instance().CreateCube(10);
	mRigidBody = PhysicsSystem::Instance().CreateRigidBody(BulletShapeType::BOX, { 10,10,10 }, pos);
}


RigidCube::~RigidCube()
{
}

void RigidCube::AsyncTransform()
{
	auto trans = mRigidBody->GetWorldTransform();
	std::vector<DirectX::XMFLOAT4X4> mat = { trans };
	mCubeCtrl->SetInstancingMatrix(mat, 0, 0);
}

void RigidCube::Draw()
{
	mCubeCtrl->DrawShadowmap();
	mCubeCtrl->DrawShadow();
}

std::shared_ptr<PrimitiveController> RigidCube::GetCtrl()
{
	return mCubeCtrl;
}
