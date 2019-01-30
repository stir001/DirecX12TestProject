#include "CollisionChecker.h"
#include "CollidableObject.h"
#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include <Dx12MSLib.h>


CollisionChecker::CollisionChecker()
{
}


CollisionChecker::~CollisionChecker()
{
}

void CollisionChecker::SetMainCollision(std::shared_ptr<CollidableObject> collision)
{
	mMainCollision = collision;
}

void CollisionChecker::SetOtherCollision(std::shared_ptr<CollidableObject> collision)
{
	mOtherCollision = collision;
}

bool CollisionChecker::IsCollide() const
{
	return IsCollide(mMainCollision, mOtherCollision);
}

bool CollisionChecker::IsCollide(const std::shared_ptr<const CollidableObject> col1, const std::shared_ptr<const CollidableObject> col2) const
{
	return col1->IsCollide(shared_from_this(), col2);
}

bool CollisionChecker::IsCollide(const CapsuleCollider& cap, const std::shared_ptr<const CollidableObject> col2) const
{
	return col2->IsCollide(shared_from_this(), cap);
}

bool CollisionChecker::IsCollide(const SphereCollider& sph, const std::shared_ptr<const CollidableObject> col2) const
{
	return col2->IsCollide(shared_from_this(), sph);
}

bool CollisionChecker::IsCollide(const CapsuleCollider& cap1, const CapsuleCollider& cap2) const
{
	return IsCapsuleCapsuleCollide(cap1, cap2);
}

bool CollisionChecker::IsCollide(const CapsuleCollider& cap, const SphereCollider& sph) const
{
	return IsCapsuleSphereCollide(cap, sph);
}

bool CollisionChecker::IsCollide(const SphereCollider& sph, const CapsuleCollider& cap) const
{
	return IsCapsuleSphereCollide(cap, sph);
}

bool CollisionChecker::IsCollide(const SphereCollider& sph1, const SphereCollider& sph2) const
{
	return IsSphereSphereCollide(sph1, sph2);
}

bool CollisionChecker::IsCapsuleSphereCollide(const CapsuleCollider& cap, const SphereCollider& sph) const
{
	bool isCollide = false;

	auto capData = cap.GetCapsuleData();
	auto sphData = sph.GetSphereData();

	auto capVec = capData.pos1 - capData.pos2;
	auto capLength = GetLengthXMFloat3(capVec);
	auto capToSphVec = sphData.origin - capData.pos2;

	auto capVecUnit = NormalizeXMFloat3(capVec);

	auto projRatio = DotXMFloat3(capVecUnit, capToSphVec);

	float length = 0.0f;

	if (projRatio / capLength < 0.0f)
	{
		length = GetLengthXMFloat3(sphData.origin - capData.pos2);
	}
	else if (projRatio / capLength > 1.0f)
	{
		length = GetLengthXMFloat3(sphData.origin - capData.pos1);
	}
	else
	{
		length = GetLengthXMFloat3(projRatio * capVecUnit - capToSphVec);
	}

	return length < sphData.radius + capData.radius;
}

bool CollisionChecker::IsCapsuleCapsuleCollide(const CapsuleCollider& cap1, const CapsuleCollider& cap2) const
{
	return false;
}

bool CollisionChecker::IsSphereSphereCollide(const SphereCollider& sph1, const SphereCollider& sph2) const
{
	return false;
}
