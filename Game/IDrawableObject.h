#pragma once
#include <memory>
#include <DirectXMath.h>

class ImageController;

class IDrawableObject
{
public:
	IDrawableObject();
	IDrawableObject(std::shared_ptr<ImageController>& imgCtrl);
	virtual ~IDrawableObject();

	void SetImageController(std::shared_ptr<ImageController>& imgCtrl);

	virtual void Draw() const = 0;
	virtual void Draw(const DirectX::XMFLOAT3& offset);
protected:
	std::shared_ptr<ImageController> mImgCtrl;
};

