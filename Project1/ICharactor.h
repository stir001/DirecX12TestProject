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
struct ActionRect;

class ICharactor :
	public IDrawableObject
{
public:
	ICharactor();
	ICharactor(std::shared_ptr<ImageController>& imgCtrl);
	virtual ~ICharactor();

	virtual void OnGround(float GroundLine) = 0;
	virtual void Update() = 0;
	virtual void OnDamage() = 0;
	
	bool IsTurn() const;

	const std::vector<ActionRect>& GetCurrentActionRects() const;
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

