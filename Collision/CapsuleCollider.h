#pragma once
#include "CollidableObject.h"
class CapsuleCollider :
	public CollidableObject
{
public:
	CapsuleCollider(float radius, float length);
	~CapsuleCollider();

	virtual bool IsCollide(const std::shared_ptr<const CollisionChecker>& checker, const std::shared_ptr<const CollidableObject> collision) const;
	virtual bool IsCollide(const std::shared_ptr<const CollisionChecker>& checker, const CapsuleCollider& capsule) const;
	virtual bool IsCollide(const std::shared_ptr<const CollisionChecker>& checker, const SphereCollider& sphere) const;

	struct CapsuleData
	{	
		float radius;
		DirectX::XMFLOAT3 pos1;
		DirectX::XMFLOAT3 pos2;
		CapsuleData() : radius(0.0f), pos1{ 0.0f, 0.0f, 0.0f }, pos2{ 0.0f, 0.0f, 0.0f } {}
		CapsuleData(float radius, const DirectX::XMFLOAT3& pos1, const DirectX::XMFLOAT3& pos2) : radius(radius), pos1(pos1), pos2(pos2) {}
	};

	CapsuleData GetCapsuleData() const;
private:
	CapsuleData mData;
};

