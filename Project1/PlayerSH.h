#pragma once
#include "IDrawableObject.h"
#include <DirectXMath.h>
#include <memory>

class ImageController;
class DxInput;

class PlayerSH :public IDrawableObject
{
private:
	std::shared_ptr<DxInput> mInput;
	DirectX::XMFLOAT3 mPos;
	DirectX::XMFLOAT3 mVel;

	void Move();
public:
	void Update();
	void Draw();
	const DirectX::XMFLOAT3& GetPlayerPos() const;
	void OnGround(float grandLine);

	PlayerSH(std::shared_ptr<ImageController> imgCtrl,std::shared_ptr<DxInput> dlibInput);
	~PlayerSH();
};

