#include "BackGround.h"
#include "PlayerSH.h"

const float GROUND_LINE = 200;

BackGround::BackGround(ImageController* imgCtrl, std::shared_ptr<PlayerSH> spPlayer):IDrawableObject(imgCtrl),mGroundLine(GROUND_LINE)
{
}

BackGround::~BackGround()
{
}

void BackGround::Update()
{
	if (IsGroundPlayer())
	{

	}
}

void BackGround::Draw()
{

}

bool BackGround::IsGroundPlayer() const
{
	if (mwpPlayer.lock()->GetPlayerPos().y <= mGroundLine)
	{
		return true;
	}
	return false;
}
