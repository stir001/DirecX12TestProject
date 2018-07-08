#pragma once
#include "ICharactor.h"
#include <DirectXMath.h>


class ImageController;
class DxInput;
struct Action;
struct ActionData;

class PlayerSH :public ICharactor
{
public:
	PlayerSH(std::shared_ptr<ImageController>& imgCtrl, std::shared_ptr<DxInput> dlibInput);
	~PlayerSH();

	void Update();
	void Draw() const;
	void Draw(const DirectX::XMFLOAT3& offset);
	void OnGround(float grandLine);
	void OnDamage();
private:
	std::shared_ptr<DxInput> mInput;

	void (PlayerSH::*mActionUpdate)();

	void Walk();
	void Neutral();
	void Jump();
	void Ground();
	void Crouch();
	void Punch();
	void Kick();
	void Sliding();
	void Damage();
	void Gravity();
};

