#pragma once
#include <memory>

class PrimitiveController;

class CollidableObject
{
public:
	CollidableObject();
	virtual ~CollidableObject();
protected:
	std::shared_ptr<PrimitiveController> mPrimitive;
};

