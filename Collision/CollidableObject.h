#pragma once
#include <memory>
#include <DirectXMath.h>

class PrimitiveController;
class CollisionChecker;
class CapsuleCollider;
class SphereCollider;
class DxInput;

class CollidableObject : public std::enable_shared_from_this<CollidableObject>
{
public:
	CollidableObject();
	virtual ~CollidableObject();
	
	virtual bool IsCollide(const std::shared_ptr<const CollisionChecker>& checker, const std::shared_ptr<const CollidableObject> collision) const = 0;
	virtual bool IsCollide(const std::shared_ptr<const CollisionChecker>& checker, const CapsuleCollider& capsule) const = 0;
	virtual bool IsCollide(const std::shared_ptr<const CollisionChecker>& checker, const SphereCollider& sphere) const = 0;

	void Draw();
	void DefaultMove(const DxInput& input);
	void SetPos(const DirectX::XMFLOAT3& pos);
	void AddPos(const DirectX::XMFLOAT3& offset);
	void SetVel(const DirectX::XMFLOAT3& vel);
	void AddRotateY(float deg);
	void AddRotateX(float deg);
	void AddRotateZ(float deg);
	void SetColor(const DirectX::XMFLOAT3& color);
	DirectX::XMFLOAT3 GetPos() const;
protected:
	std::shared_ptr<PrimitiveController> mPrimitive;
	DirectX::XMFLOAT3 mPos;
	DirectX::XMFLOAT3 mVel;
};

