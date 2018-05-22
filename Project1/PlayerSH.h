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

	PlayerSH(std::shared_ptr<ImageController> imgCtrl, std::shared_ptr<DxInput> dlibInput);
	~PlayerSH();

	void Update();
	void Draw();
	void OnGround(float grandLine);
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
	void Gravity();
};

