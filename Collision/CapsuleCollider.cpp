#include "CapsuleCollider.h"
#include "CollisionChecker.h"
#include <Dx12MSLib.h>

CapsuleCollider::CapsuleCollider(float radius, float length) : mData(radius, DirectX::XMFLOAT3(0.0f, length * 0.5f, 0.0f) , DirectX::XMFLOAT3( 0.0f, -length * 0.5f, 0.0f))
{
	mPrimitive = PrimitiveCreator::Instance().CreateCapsule(radius, length);
}


CapsuleCollider::~CapsuleCollider()
{
}

bool CapsuleCollider::IsCollide(const std::shared_ptr<const CollisionChecker>& checker, const std::shared_ptr<const CollidableObject> collision) const
{
	return collision->IsCollide(checker, *this);
}

bool CapsuleCollider::IsCollide(const std::shared_ptr<const CollisionChecker>& checker, const CapsuleCollider & capsule) const
{
	return checker->IsCollide(*this, capsule);
}

bool CapsuleCollider::IsCollide(const std::shared_ptr<const CollisionChecker>& checker, const SphereCollider & shpere) const
{
	return checker->IsCollide(*this, shpere);
}

CapsuleCollider::CapsuleData CapsuleCollider::GetCapsuleData() const
{
	auto rtn = mData;
	auto mat = mPrimitive->GetMatrix();
	rtn.pos1 *= mat;
	rtn.pos2 *= mat;
	return rtn;
}
