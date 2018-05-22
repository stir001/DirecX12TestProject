#pragma once
#include "IDrawableObject.h"
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <DirectXMath.h>

class ImageController;
struct Action;
struct ActionData;

class ICharactor :
	public IDrawableObject
{
public:
	ICharactor();
	ICharactor(std::shared_ptr<ImageController> imgCtrl);
	virtual ~ICharactor();


	void SetAction(ActionData& inActs);
	const DirectX::XMFLOAT3& GetPos() const;
protected:
	std::vector<Action> mActions;
	DirectX::XMFLOAT3 mPos;
	DirectX::XMFLOAT3 mVel;
	std::vector<Action>::iterator mCurrentAction;
	int mFrame;
	int mActionImageIndex;
	bool mIsturn;
	std::function<void(void)> mChangeNextAction;
	void UpdatePostion();
	void ChangeAction(const char* actionName);
	void SetActionImageData();
	void AnimationUpdate();
};

