#include "Dx12Ctrl.h"
#include "Geometry.h"
#include "PMDLoader.h"
#include "PrimitiveManager.h"
#include "PMDController.h"
#include "VMDLoader.h"
#include "Dx12Camera.h"
#include "DxInput.h"
#include "DirectionalLight.h"
#include "ImageLoader.h"
#include "ImageObject.h"

#include <algorithm>
#include <Windows.h>
#include <tchar.h>
#include <d3d12.h>
#include "d3dx12.h"
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <map>

using namespace DirectX;

const int WINDOW_WIDTH = 768;
const int WINDOW_HEIGHT = 448;
const TCHAR* APP_CLASS_NAME = _T("DirectX12Test");
const int SCREEN_BUFFER_COUNT = 2;

//LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	///Direct3D12‚Ì‰Šú‰»

	HRESULT result = S_OK;

	Dx12Ctrl* d12 = Dx12Ctrl::Instance();
	d12->SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	std::string wName = "SplatterHouse";
	d12->SetWindowName(wName);
	d12->Dx12Init();
	
	ImageLoader imgLoader;
	//std::string imgpath = "gollira.png";
	std::string imgpath = "Action18/img/splatterhouse.png";
	std::string playerImgpath = "Action18/img/rick.png";
	ImageObject* imgObject = imgLoader.LoadImageData(imgpath);
	ImageObject* player = imgLoader.LoadImageData(playerImgpath);

	DxInput input;

	MousePos pos;
	XMFLOAT3 cPos = { 0,20,-30 };

	Dx12Camera* camera = d12->GetCamera();

	float rota = 1.0f;
	float scale = 1.0f;
	DirectX::XMFLOAT3 imgpos = { 0,0,0 };
	DirectX::XMFLOAT3 t_pos{ 32,32,0 };
	player->SetRect(t_pos, 64, 64);
	player->SetScale(2.0f);
	//player->Turn();
	//ƒƒCƒ“ƒ‹[ƒv
	while (ProcessMessage()) {
		CallStartPerGameLoop();

		input.UpdateKeyState();

		pos = input.GetMousePos();

		if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_LSHIFT))
		{
			scale *= 1.1f;
			imgObject->SetScale(scale);
		}

		if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_SPACE))
		{
			scale *= 0.9f;
			imgObject->SetScale(scale);
		}

		if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_W))
		{
			imgpos.y += 10.0f;
			imgObject->SetPos(imgpos);
		}

		if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_S))
		{
			imgpos.y -= 10.0f;
			imgObject->SetPos(imgpos);
		}

		if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_A))
		{
			imgpos.x -= 10.0f;
			imgObject->SetPos(imgpos);
		}

		if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_D))
		{
			imgpos.x += 10.0f;
			imgObject->SetPos(imgpos);
		}

		if (input.IsKeyTrigger(eVIRTUAL_KEY_INDEX_LEFT))
		{
			player->Turn();
		}

		if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_ALT))
		{
			player->Turn();
		}

		if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_UP))
		{
			rota += 1.0f;
			imgObject->SetRota(rota);
		}

		if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_DOWN))
		{
			rota -= 1.0f;
			imgObject->SetRota(rota);
		}

		player->Draw();
		imgObject->Draw();

		CallEndPerGameLoop();
	}
	d12->Release();
#ifdef _DEBUG
	ID3D12DebugDevice* debugdev = nullptr;
	d12->GetDev()->QueryInterface(&debugdev);
	
	debugdev->ReportLiveDeviceObjects(D3D12_RLDO_FLAGS::D3D12_RLDO_DETAIL);
#endif

}
