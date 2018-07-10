#include "Dx12Ctrl.h"
#include "RenderingPathManager.h"
#include "AnimationPlayerManager.h"
#include <thread>

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
	//std::thread threadObject([](AnimationPlayerManager* manager)
	//{
	//	manager->UpdateAnimations();
	//}, AnimationPlayerManager::Instance());

	//threadObject.join();
	AnimationPlayerManager::Instance()->UpdateAnimations();
<<<<<<< HEAD:Project1/DirectX12/DirectX12/NeedCall.cpp
=======
	d12.InitMainCmdList();
>>>>>>> f1bde25f671e4006252ea9c7cb50882dbc0ff550:Project1/Geometry.cpp
}

void CallEndPerGameLoop()
{
<<<<<<< HEAD:Project1/DirectX12/DirectX12/NeedCall.cpp
	//AnimationPlayerManager::Instance()->WaitSafeFree();
	RenderingPathManager::Instance()->Render();
=======
	DX12CTRL_INSTANCE
	d12.ExcuteAndPresent();
>>>>>>> f1bde25f671e4006252ea9c7cb50882dbc0ff550:Project1/Geometry.cpp
}

bool ProcessMessage()
{
	MSG msg = {};
	bool rtn = true;
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT)
	{
		rtn = false;
	}

	return rtn;
}