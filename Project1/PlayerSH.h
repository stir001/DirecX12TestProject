#pragma once
#include "IDrawableObject.h"
#include <DirectXMath.h>
#include <memory>

class ImageController;
class DxLibInput;

class PlayerSH :public IDrawableObject
{
private:
	std::shared_ptr<DxLibInput> mInput;
public:
	void Update();
	void Draw();

	PlayerSH(ImageController* imgCtrl,std::shared_ptr<DxLibInput> dlibInput);
	~PlayerSH();
};

