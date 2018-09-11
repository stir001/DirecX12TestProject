#include "Dx12Ctrl.h"
#include "NeedCall.h"
#include "Dx12Camera.h"
#include "DxInput.h"
#include "XMFloatOperators.h"

#include <Windows.h>
#include <d3d12.h>
#include "d3dx12.h"
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <map>
#include <memory>
#include "btBulletDynamicsCommon.h"
#include "BulletlibLink.h"

using namespace DirectX;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	///Direct3D12‚Ì‰Šú‰»
	Dx12CtrlInit(hInst);

	auto& camera = Dx12Ctrl::Instance().GetCamera();
	DxInput input;

	clock_t sub = 0;
	clock_t preClock = 0;
	clock_t currentClock = 0;
	while (ProcessMessage()) {
		CallStartPerGameLoop();
		input.UpdateKeyState();

		camera->DefaultMove(input);

		CallEndPerGameLoop();
	}

	camera = nullptr;

	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
}
