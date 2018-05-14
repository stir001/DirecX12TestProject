#pragma once
#include "IDrawableObject.h"
#include <memory>
#include <DirectXMath.h>

class ImageController;
class PlayerSH;

class BackGround :public IDrawableObject
{
public:
	void Update();
	void Draw();
	BackGround(std::shared_ptr<ImageController> imgCtrl,std::shared_ptr<PlayerSH> spPlayer);
	~BackGround();

private:
	float mGroundLine;

	std::shared_ptr<ImageController> mSecondImage;
	DirectX::XMFLOAT3 mPos;
	std::weak_ptr<PlayerSH> mwpPlayer;

	bool IsGroundPlayer() const;
};

