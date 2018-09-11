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
	AnimationPlayerManager::Instance().UpdateAnimations();
}

void CallEndPerGameLoop()
{
	AnimationPlayerManager::Instance().WaitSafeFree();
	RenderingPathManager::Instance().Render();
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

const int WINDOW_WIDTH = 768;
const int WINDOW_HEIGHT = 448;

void Dx12CtrlInit(HINSTANCE hInst)
{
	Dx12Ctrl::Instance().SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	std::string wName = "DirectX12";
	Dx12Ctrl::Instance().SetWindowName(wName);
	Dx12Ctrl::Instance().Dx12Init(hInst);
}