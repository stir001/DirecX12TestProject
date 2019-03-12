#pragma once
#include <memory>

class PrimitiveController;
class BulletRigidBody;

class RigidCube
{
public:
	RigidCube();
	~RigidCube();

	void AsyncTransform();

	void Draw();

	void SetFriction(float friction);

	std::shared_ptr<PrimitiveController> GetCtrl();
private:
	std::shared_ptr<PrimitiveController> mCubeCtrl;
	std::shared_ptr<BulletRigidBody> mRigidBody;
};

