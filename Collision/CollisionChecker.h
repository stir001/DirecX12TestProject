#pragma once
#include <memory>

class CollidableObject;
class CapsuleCollider;
class SphereCollider;

class CollisionChecker : std::enable_shared_from_this<CollisionChecker>
{
public:
	CollisionChecker();
	~CollisionChecker();
	void SetMainCollision(std::shared_ptr<CollidableObject> collision);
	void SetOtherCollision(std::shared_ptr<CollidableObject> collision);
	bool IsCollide() const;

	bool IsCollide(const std::shared_ptr<const CollidableObject> col1, const std::shared_ptr<const CollidableObject> col2) const;

	//片方が抽象化
	bool IsCollide(const CapsuleCollider& cap, const std::shared_ptr<const CollidableObject> col2) const;
	bool IsCollide(const SphereCollider& sph, const std::shared_ptr<const CollidableObject> col2) const;

	//カプセル
	bool IsCollide(const CapsuleCollider& cap1, const CapsuleCollider& cap2) const;
	bool IsCollide(const CapsuleCollider& cap, const SphereCollider& sph) const;

	//スフィア
	bool IsCollide(const SphereCollider& sph, const CapsuleCollider& cap) const;
	bool IsCollide(const SphereCollider& sph1, const SphereCollider& sph2) const;

private:

	bool IsCapsuleSphereCollide(const CapsuleCollider& cap, const SphereCollider& sph) const;
	bool IsCapsuleCapsuleCollide(const CapsuleCollider& cap1, const CapsuleCollider& cap2) const;
	bool IsSphereSphereCollide(const SphereCollider& sph1, const SphereCollider& sph2) const;

	std::shared_ptr<CollidableObject> mMainCollision;
	std::shared_ptr<CollidableObject> mOtherCollision;

};

