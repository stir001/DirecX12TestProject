#pragma once
#include <memory>
#include <string>

class PrimitiveController;
class Transform3DCalculator;
class DxInput;

class NormalMapCube
{
public:
	NormalMapCube(float length, std::string& normalMapPath);
	~NormalMapCube();

	void Roatation(DxInput& input);
	void Draw();
private:
	std::shared_ptr<PrimitiveController> mCubeCtrl;
	std::shared_ptr<Transform3DCalculator> mCalculator;
	float mRotaDeg;
};

