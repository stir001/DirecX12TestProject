#include "BulletRigidBody.h"
#include "bullet/System/PhysicsSystem.h"


BulletRigidBody::BulletRigidBody() : mMass(1.0f), mTag(-1), mLocalScale(0.5f)
{
}

BulletRigidBody::BulletRigidBody(std::shared_ptr<btCollisionShape> collisionShape
	, const DirectX::XMFLOAT3& pos)
	: mCollisionShape(collisionShape)
	, mMass(1.0f), mTag(-1), mLocalScale(0.5f)
{
	CreateRigidBody();
	Translate(pos.x, pos.y, pos.z);
}


BulletRigidBody::~BulletRigidBody()
{
}

std::shared_ptr<btRigidBody> BulletRigidBody::GetRigidBody()
{
	return mRigidBody;
}

void BulletRigidBody::SetMass(float mass)
{
	mMass = mass;
	btVector3 bodyInertia;
	mCollisionShape->calculateLocalInertia(mMass, bodyInertia);
	mRigidBody->setMassProps(mMass, bodyInertia);
}

void BulletRigidBody::Translate(float x, float y, float z)
{
	mRigidBody->translate(btVector3(x, y, z));
}

void BulletRigidBody::Translate(const DirectX::XMFLOAT3 & pos)
{
	Translate(pos.x, pos.y, pos.z);
}

DirectX::XMFLOAT4X4 BulletRigidBody::GetWorldTransform() const
{
	btTransform transform;
	mMotionState->getWorldTransform(transform);
	btScalar mat[16];
	transform.getOpenGLMatrix(mat);
	DirectX::XMFLOAT4X4 rtn{
		(float)mat[0], (float)mat[1], (float)mat[2] , (float)mat[3]
		, (float)mat[4], (float)mat[5], (float)mat[6], (float)mat[7]
		, (float)mat[8], (float)mat[9], (float)mat[10], (float)mat[11]
		, (float)mat[12] / mLocalScale, (float)mat[13] / mLocalScale, (float)mat[14] / mLocalScale, (float)mat[15]
	};

	return rtn;
}

void BulletRigidBody::SetWorldTransform(const DirectX::XMFLOAT4X4& matrix)
{
	btScalar mat[16] =
	{
		matrix._11, matrix._12, matrix._13, matrix._14
		, matrix._21, matrix._22, matrix._23, matrix._24
		, matrix._31, matrix._32, matrix._33, matrix._34
		, matrix._41 * mLocalScale, matrix._42 * mLocalScale, matrix._43 * mLocalScale, matrix._44
	};
	btTransform transform;
	transform.setOrigin(btVector3(matrix._41 * mLocalScale, matrix._42 * mLocalScale, matrix._43 * mLocalScale));
	transform.setFromOpenGLMatrix(mat);
	mMotionState->setWorldTransform(transform);
}

int BulletRigidBody::GetTag() const
{
	return mTag;
}

void BulletRigidBody::CreateRigidBody(bool isContlrable)
{
	mCollisionShape->setLocalScaling(btVector3(mLocalScale, mLocalScale, mLocalScale));
	mMotionState = std::make_shared<btDefaultMotionState>();
	btVector3 bodyInertia;
	mCollisionShape->calculateLocalInertia(mMass, bodyInertia);
	auto bodyCI = btRigidBody::btRigidBodyConstructionInfo(mMass, mMotionState.get(), mCollisionShape.get(), bodyInertia);

	bodyCI.m_restitution = 1.0f;
	bodyCI.m_friction = 0.5f;

	mRigidBody = std::make_shared<btRigidBody>(bodyCI);
	if (isContlrable)
	{
		mRigidBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
		mRigidBody->setActivationState(DISABLE_DEACTIVATION);
	}
}


