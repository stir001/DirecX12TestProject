#pragma once
#include <memory>
#include <string>
#include <vector>
#include <DirectXMath.h>

class PrimitiveController;
class Transform3DCalculator;
class DxInput;
class BulletRigidBody;

class NormalMapCube
{
public:
	NormalMapCube(float length, const std::string& normalMapPath);
	~NormalMapCube();

	void AsyncRigidBody();
	void Roatation(DxInput& input);
	void Translate(const DirectX::XMFLOAT3& pos, int index = 0);
	void Translate(float x, float y, float z, int index = 0);
	void Instanceing(const DirectX::XMFLOAT3& pos, float length = -1);
	void Draw();
private:
	void Create(float length);

	std::shared_ptr<PrimitiveController> mCubeCtrl;
	std::shared_ptr<Transform3DCalculator> mCalculator;
	std::vector<DirectX::XMFLOAT4X4> mInstanceMatrix;
	std::vector<std::shared_ptr<BulletRigidBody>> mRigidBody;
	std::string mNormalMap;
	float mRotaDeg;
	float mLength;
};

