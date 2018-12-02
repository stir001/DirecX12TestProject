#include "SphereCollisionShape.h"



SphereCollisionShape::SphereCollisionShape(float radius)
{
	mCollisionShape = std::make_shared<btSphereShape>(radius);
}


SphereCollisionShape::~SphereCollisionShape()
{
}