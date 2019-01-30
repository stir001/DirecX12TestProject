#pragma once
#include "CollidableObject.h"
class SphereCollider :
	public CollidableObject
{
public:
	SphereCollider(float radius);
	~SphereCollider();

	virtual bool IsCollide(const std::shared_ptr<const CollisionChecker>& checker, const std::shared_ptr<const CollidableObject> collision) const;
	virtual bool IsCollide(const std::shared_ptr<const CollisionChecker>& checker, const CapsuleCollider& capsule) const;
	virtual bool IsCollide(const std::shared_ptr<const CollisionChecker>& checker, const SphereCollider& sphere) const;

	struct SphereData
	{
		float radius;
		DirectX::XMFLOAT3 origin;
		SphereData() : radius(0.0f), origin{ 0.0f, 0.0f, 0.0f } {}
		SphereData(float radius, DirectX::XMFLOAT3 origin) : radius(radius), origin(origin){}
	};

	SphereData GetSphereData() const;
private:
	SphereData mData;
};

