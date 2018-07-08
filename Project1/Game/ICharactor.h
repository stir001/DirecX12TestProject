#pragma once
#include "IDrawableObject.h"
#include <vector>
#include <string>
#include <functional>
#include <DirectXMath.h>

class ImageController;
struct Action;
struct ActionData;
struct ActionRect;

#ifndef _CHARACTOR_SCALE_
const float CHARACTOR_SCALE = 2.0f;
#endif // !_CHARACTOR_SCALE_

#ifdef _DEBUG
class Primitive2DLine;
#endif // _DEBUG


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
	virtual void Draw() const = 0;

	virtual void Draw(const DirectX::XMFLOAT3& offset);
	
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

#ifdef _DEBUG
	void DrawRect() const;
#endif // _DEBUG
private:
#ifdef _DEBUG
	void UpDateRectLine();
	void UpDateRectPosition();
	std::vector<std::shared_ptr<Primitive2DLine>> mRectLines;
#endif // _DEBUG

};

