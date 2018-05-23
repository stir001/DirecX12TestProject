#pragma once
#include "IDrawableObject.h"
#include <memory>
#include <DirectXMath.h>
#include <vector>

class ImageController;
class ICharactor;

class BackGround :public IDrawableObject
{
public:
	void Update();
	void Draw() const ;
	BackGround(std::shared_ptr<ImageController> imgCtrl,std::shared_ptr<PlayerSH> spPlayer);
	~BackGround();

private:
	float mGroundLine;

	std::shared_ptr<ImageController> mSecondImage;
	DirectX::XMFLOAT3 mPos;
	std::vector<std::weak_ptr<ICharactor>> mwpCharactor;

	bool IsGroundCharactor(std::weak_ptr<ICharactor> charactor) const;
};

