#pragma once
#include <memory>

class PrimitiveController;
class PrimitiveObject;

class Star
{
public:
	Star(std::shared_ptr<PrimitiveObject> primitive);
	~Star();
private:
	std::shared_ptr<PrimitiveController> mStar;
};

