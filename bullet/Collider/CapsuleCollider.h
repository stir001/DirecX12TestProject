#pragma once
#include "Collider.h"
class CapsuleCollider :
	public Collider
{
public:
	CapsuleCollider(float radius, float height);
	~CapsuleCollider();
private:
	std::shared_ptr<btCapsuleShape> mCapsuleShape;
};

