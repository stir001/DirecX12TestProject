#include "Dx12Ctrl.h"
#include "AnimationPlayerManager.h"

unsigned int Roundup2Multiplier(unsigned int size)
{
	unsigned int bit = 0x8000000;//32bit‚ÌŽž
	for (unsigned int i = 31; i >= 0; i--)
	{
		if (size & bit)break;
		bit >>= 1;
	}

	return size + ((bit << 1) - size) % bit;
}


void CallStartPerGameLoop()
{
	DX12CTRL_INSTANCE
	AnimationPlayerManager::Instance()->UpdateAnimations();
	d12->InitMainCmdList();
}

void CallEndPerGameLoop()
{
	DX12CTRL_INSTANCE
	d12->ExcuteAndPresent();
}
