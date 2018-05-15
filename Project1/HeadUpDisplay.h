#pragma once
#include "IDrawableObject.h"
#include <DirectXMath.h>

class ImageController;

class HeadUpDisplay :
	public IDrawableObject
{
public:
	HeadUpDisplay(std::shared_ptr<ImageController> imgCtrl);
	~HeadUpDisplay();

	void SetPos(DirectX::XMFLOAT3& pos);
	void SetPos(float x, float y, float z);
	void Draw();
private:
	DirectX::XMFLOAT3 mPos;
};

