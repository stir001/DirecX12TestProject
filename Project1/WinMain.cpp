#include "Dx12Ctrl.h"
#include "GameScene.h"
#include "DxInput.h"
#include "Primitive2DLine.h"
#include "NeedCall.h"

#include <Windows.h>
#include <tchar.h>
#include <memory>


using namespace DirectX;

const int WINDOW_WIDTH = 768;
const int WINDOW_HEIGHT = 448;
const TCHAR* APP_CLASS_NAME = _T("DirectX12Test");
const int SCREEN_BUFFER_COUNT = 2;


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	///Direct3D12の初期化

	Dx12Ctrl* d12 = Dx12Ctrl::Instance();
	d12->SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	std::string wName = "1601295_真鍋奨一郎";
	d12->SetWindowName(wName);
	d12->Dx12Init(hInst);
	
	std::shared_ptr<DxInput> spInput(new DxInput());
	GameScene* gameScene = new GameScene(spInput);

	//メインループ
	while (ProcessMessage()) {
		CallStartPerGameLoop();
		gameScene->Run();

		CallEndPerGameLoop();
	}
	d12->Release();
	Dx12Ctrl::Destroy();
}
