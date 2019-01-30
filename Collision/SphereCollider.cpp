#include "SphereCollider.h"
#include "CollisionChecker.h"
#include <Dx12MSLib.h>

SphereCollider::SphereCollider(float radius) : mData(radius, {0.0f, 0.0f, 0.0f})
{
	mPrimitive = PrimitiveCreator::Instance().CreateSphere(radius, 100);
}


SphereCollider::~SphereCollider()
{
}

bool SphereCollider::IsCollide(const std::shared_ptr<const CollisionChecker>& checker, const std::shared_ptr<const CollidableObject> collision) const
{
	return collision->IsCollide(checker, *this);
}

bool SphereCollider::IsCollide(const std::shared_ptr<const CollisionChecker>& checker, const CapsuleCollider & capsule) const
{
	return checker->IsCollide(*this, capsule);
}

bool SphereCollider::IsCollide(const std::shared_ptr<const CollisionChecker>& checker, const SphereCollider & sphere) const
{
	return checker->IsCollide(*this, sphere);
}

SphereCollider::SphereData SphereCollider::GetSphereData() const
{
	auto rtn = mData;
	rtn.origin *= mPrimitive->GetMatrix();
	return rtn;
}
