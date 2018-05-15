#include "PlayerSH.h"
#include "ImageController.h"
#include "DxInput.h"
#include "XMFloat3Operators.h"

const float VELOCITY_X  = 2.0f;
const float VELOCITY_Y  = 4.0f;
const float GRAVITY		= 1.0f;

PlayerSH::PlayerSH(std::shared_ptr<ImageController> imgCtrl, std::shared_ptr<DxInput> dlibInput) :IDrawableObject::IDrawableObject(imgCtrl)
, mInput(dlibInput), mVel(0.0f, 0.0f, 0.0f), mPos(0, 0, 0),mActionUpdate(&PlayerSH::Walk)
{
	imgCtrl->SetPos(mPos);
}

PlayerSH::~PlayerSH()
{
}

void PlayerSH::Update()
{
	(this->*mActionUpdate)();
}

void PlayerSH::Draw()
{
	mImgCtrl->Draw();
}

void PlayerSH::Walk()
{
	mVel.x = 0;
	mVel.y = 0;
	mVel.x = mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_D) ?
		VELOCITY_X : mVel.x;

	mVel.x = mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_A) ?
		-VELOCITY_X : mVel.x;

	mVel.y = mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_W) ?
		VELOCITY_Y : mVel.y;

	mVel.y = mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_S) ?
		-VELOCITY_Y : mVel.y;

	mPos += mVel;

	mImgCtrl->SetPos(mPos);
}

const DirectX::XMFLOAT3& PlayerSH::GetPlayerPos() const
{
	return mPos;
}

void PlayerSH::OnGround(float grandLine)
{
	mVel.y = 0;
	mPos.y = grandLine;
}

void PlayerSH::SetAction(std::vector<Action>& inActs)
{
	//mActions.swap(inActs);
}
