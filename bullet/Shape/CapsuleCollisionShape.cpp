#include "CapsuleCollisionShape.h"



CapsuleCollisionShape::CapsuleCollisionShape(float radius, float height)
{
	mCollisionShape = std::make_shared<btCapsuleShape>(radius, height);
}


CapsuleCollisionShape::~CapsuleCollisionShape()
{
}