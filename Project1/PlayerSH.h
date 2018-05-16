#pragma once
#include "IDrawableObject.h"
#include <DirectXMath.h>
#include <memory>
#include <vector>
#include <string>
#include <functional>

class ImageController;
class DxInput;
struct Action;

class PlayerSH :public IDrawableObject
{
public:

	PlayerSH(std::shared_ptr<ImageController> imgCtrl, std::shared_ptr<DxInput> dlibInput);
	~PlayerSH();

	void Update();
	void Draw();
	const DirectX::XMFLOAT3& GetPlayerPos() const;
	void OnGround(float grandLine);
	void SetAction(std::vector<Action>& inActs);
private:
	std::vector<Action> mActions;
	std::shared_ptr<DxInput> mInput;
	DirectX::XMFLOAT3 mPos;
	DirectX::XMFLOAT3 mVel;
	std::vector<Action>::iterator mCurrentAction;
	int mFrame;
	int mActionImageIndex;
	bool mIsturn;
	void (PlayerSH::*mActionUpdate)();
	std::function<void(void)> mChangeNextAction;

	void ChangeAction(const std::string& actionName);
	void SetActionImageData();
	void AnimationUpdate();
	void Walk();
	void Neutral();
};

