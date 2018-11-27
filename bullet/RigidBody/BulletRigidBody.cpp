#include "BulletRigidBody.h"
#include "bullet/System/PhysicsSystem.h"


BulletRigidBody::BulletRigidBody() : mMass(1.0f), mTag(-1)
{
}

BulletRigidBody::BulletRigidBody(std::shared_ptr<btCollisionShape> collisionShape
	, const DirectX::XMFLOAT3& pos)
	: mCollisionShape(collisionShape)
	, mMass(1.0f), mTag(-1)
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
		mat[0], mat[4], mat[8] , mat[12]
		, mat[1], mat[5], mat[9], mat[13]
		, mat[2], mat[6], mat[10], mat[14]
		, mat[3], mat[7], mat[11], mat[15]
	};

	return rtn;
}

void BulletRigidBody::SetWorldTransform(const DirectX::XMFLOAT4X4& matrix)
{
	btScalar mat[16] =
	{
		matrix._11, matrix._21, matrix._31, matrix._41
		, matrix._12, matrix._22, matrix._32, matrix._42
		, matrix._13, matrix._23, matrix._33, matrix._43
		, matrix._14, matrix._24, matrix._34, matrix._44
	};
	btTransform transform;
	transform.setFromOpenGLMatrix(mat);
	mMotionState->setWorldTransform(transform);
}

int BulletRigidBody::GetTag() const
{
	return mTag;
}

void BulletRigidBody::CreateRigidBody()
{
	mMotionState = std::make_shared<btDefaultMotionState>();
	btVector3 bodyInertia;
	mCollisionShape->calculateLocalInertia(mMass, bodyInertia);
	auto bodyCI = btRigidBody::btRigidBodyConstructionInfo(mMass, mMotionState.get(), mCollisionShape.get(), bodyInertia);

	bodyCI.m_restitution = 1.0f;
	bodyCI.m_friction = 0.5f;

	mRigidBody = std::make_shared<btRigidBody>(bodyCI);
}


