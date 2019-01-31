#pragma once
#include <memory>
#include <DirectXMath.h>

class PrimitiveController;
class DxInput;

class PlayerCharacter
{
public:
	PlayerCharacter();
	~PlayerCharacter();

	void Draw();
	void Move(const DxInput& input);
	void SetPosition(const DirectX::XMFLOAT3& pos);
private:
	std::shared_ptr<PrimitiveController> mCone;
	std::shared_ptr<PrimitiveController> mSphere;
	DirectX::XMFLOAT3 mSphereOffset;
	DirectX::XMFLOAT3 mPos;
};

