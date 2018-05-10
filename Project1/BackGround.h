#pragma once
#include "IDrawableObject.h"
#include <memory>

class ImageController;
class PlayerSH;

class BackGround :public IDrawableObject
{
public:
	void Update();
	void Draw();
	BackGround(ImageController* imgCtrl,std::shared_ptr<PlayerSH> spPlayer);
	~BackGround();

private:
	float mGroundLine;

	std::weak_ptr<PlayerSH> mwpPlayer;

	bool IsGroundPlayer() const;
};

