#pragma once
#include <memory>
#include <DirectXMath.h>

class PrimitiveController;
class DxInput;
class Dx12Camera;

class PlayerCharacter
{
public:
	PlayerCharacter(std::shared_ptr<Dx12Camera> camera, float size);
	~PlayerCharacter();

	void Draw();
	void Move(const DxInput& input);
	void SetPosition(const DirectX::XMFLOAT3& pos);
	void SetCameraOffset(const DirectX::XMFLOAT3& offset);
	DirectX::XMFLOAT3 GetPos() const;
private:
	std::shared_ptr<PrimitiveController> mCone;
	std::shared_ptr<PrimitiveController> mSphere;
	DirectX::XMFLOAT3 mSphereOffset;
	DirectX::XMFLOAT3 mPos;
	DirectX::XMFLOAT3 mVel;
	DirectX::XMFLOAT3 mCameraOffset;
	std::shared_ptr<Dx12Camera> mCamera;
};

