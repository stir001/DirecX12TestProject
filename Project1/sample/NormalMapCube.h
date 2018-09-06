#pragma once
#include <memory>
#include <string>
#include <vector>
#include <DirectXMath.h>

class PrimitiveController;
class Transform3DCalculator;
class DxInput;

class NormalMapCube
{
public:
	NormalMapCube(float length, const std::string& normalMapPath);
	~NormalMapCube();

	void Roatation(DxInput& input);
	void Draw();
private:
	std::shared_ptr<PrimitiveController> mCubeCtrl;
	std::shared_ptr<Transform3DCalculator> mCalculator;
	std::vector<DirectX::XMFLOAT4X4> mInstanceMatrix;
	float mRotaDeg;
};

