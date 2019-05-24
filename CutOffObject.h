#pragma once
#include <memory>

struct PrimitiveController;

class CutOffObject
{
public:
	CutOffObject();
	~CutOffObject();

	void DrawBase();
	void DrawPlus();
	void DrawMinus();
private:
	std::shared_ptr<PrimitiveController> mBaseObject;
	std::shared_ptr<PrimitiveController> mPlusObjct;
	std::shared_ptr<PrimitiveController> mMinusObject;
};

