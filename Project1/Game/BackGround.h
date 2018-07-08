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
	void Draw(const DirectX::XMFLOAT3& offset);
	BackGround(std::shared_ptr<ImageController> imgCtrl,std::shared_ptr<ICharactor> spPlayer);
	~BackGround();

	void SetCharactor(std::shared_ptr<ICharactor> charactor);

private:
	float mGroundLine;

	std::shared_ptr<ImageController> mSecondImage;
	std::shared_ptr<ImageController> mThirdImage;
	DirectX::XMFLOAT3 mPos;
	bool mIsFirstTurn;
	DirectX::XMFLOAT3 mSecondPos;
	bool mIsSecondTurn;
	DirectX::XMFLOAT3 mThirdPos;
	bool mIsThirdTurn;
	std::vector<std::shared_ptr<ICharactor>> mCharactor;

	bool IsGroundCharactor(std::shared_ptr<ICharactor> charactor) const;
};

