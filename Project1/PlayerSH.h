#pragma once
#include "IDrawableObject.h"
#include <DirectXMath.h>
#include <memory>
#include <vector>

class ImageController;
class DxInput;
struct Action;

class PlayerSH :public IDrawableObject
{
private:
	std::vector<Action> mActions;
	std::shared_ptr<DxInput> mInput;
	DirectX::XMFLOAT3 mPos;
	DirectX::XMFLOAT3 mVel;
	void (PlayerSH::*mActionUpdate)();
		
	void Walk();
public:
	void Update();
	void Draw();
	const DirectX::XMFLOAT3& GetPlayerPos() const;
	void OnGround(float grandLine);
	void SetAction(std::vector<Action>& inActs);

	PlayerSH(std::shared_ptr<ImageController> imgCtrl,std::shared_ptr<DxInput> dlibInput);
	~PlayerSH();
};

