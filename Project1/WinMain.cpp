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

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const TCHAR* APP_CLASS_NAME = _T("DirectX12Test");
const int SCREEN_BUFFER_COUNT = 2;

//LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	///Direct3D12の初期化

	HRESULT result = S_OK;

	Dx12Ctrl::Instance()->Dx12Init();
	Dx12Ctrl* d12 = Dx12Ctrl::Instance();

	//PMXLoader loader;
	//TgaLoader tgaLoader;
	//std::string tgapath = "Tda式ミク/k_face_MikuAp.tga";
	//tgaLoader.LoadImageData(tgapath);

	//std::string path = "Tda式ミク/Tda式ミク.pmx";
	//loader.Load(path);

	PMDLoader pmdloader;
	std::string modelpath = "初音ミク.pmd";
	//std::string modelpath = "博麗霊夢/reimu_F01.pmd";
	PMDController* model = pmdloader.Load(modelpath);

	MSG msg = {};

	//std::string vmdPath = "vmd/charge.vmd";

	//VMDLoader vmdloader;
	//VMDMotion* motion = vmdloader.LoadMotion(vmdPath);
	//model->SetMotion(motion);
	//model->PlayMotion(true);
	XMFLOAT3 t_lightpos = { -20,0,-20 };
	XMFLOAT3 t_lightdir = { 1.5f,-0.5f,1.0f };
	DirectionalLight* dirLight = new DirectionalLight(t_lightpos, t_lightdir);

	ImageLoader imgLoader;
	std::string imgpath = "Action18/img/splatterhouse.png";
	std::string playerImagePath = "Action18/img/rick.png";
	ImageObject* imgObject = imgLoader.LoadImageData(imgpath);
	ImageObject* player = imgLoader.LoadImageData(playerImagePath);

	PrimitiveManager priMgr;
	priMgr.CreatePlane(XMFLOAT3(0, 0, 0), 60, 60, XMFLOAT3(0, 1, 0));

	priMgr.CreatePlane(XMFLOAT3(0, 30, 30), 60, 60, XMFLOAT3(0, 0, -1));

	priMgr.CreatePlane(XMFLOAT3(30, 30, 0), 60, 60, XMFLOAT3(-1, 0, 0));
	
	priMgr.SetLightObject(dirLight);

	DxInput input;

	MousePos pos;
	XMFLOAT3 cPos = { 0,20,-30 };

	Dx12Camera* camera = d12->GetCamera();

	float rota = 1.0f;
	float scale = 1.0f;
	DirectX::XMFLOAT3 imgpos = { 0,0,0 };
	//メインループ
	while (true) {
		CallStartPerGameLoop();

		input.UpdateKeyState();

		pos = input.GetMousePos();

		if (input.GetKeyState()[vki_LSHIFT] & ksc_DOWN)
		{
			scale *= 1.1f;
			imgObject->SetScale(scale);
			//camera->MoveUp(-0.1f);
		}

		if (input.GetKeyState()[vki_SPACE] & ksc_DOWN)
		{
			scale *= 0.9f;
			imgObject->SetScale(scale);
			//camera->MoveUp(0.1f);
		}

		if (input.GetKeyState()[vki_W] & ksc_DOWN)
		{
			//camera->MoveFront(0.1f);
			imgpos.y += 10.0f;
			imgObject->SetPos(imgpos);
		}

		if (input.GetKeyState()[vki_S] & ksc_DOWN)
		{
			//camera->MoveFront(-0.1f);
			imgpos.y -= 10.0f;
			imgObject->SetPos(imgpos);
		}

		if (input.GetKeyState()[vki_A] & ksc_DOWN)
		{
			//camera->MoveSide(0.1f);
			imgpos.x -= 10.0f;
			imgObject->SetPos(imgpos);
		}

		if (input.GetKeyState()[vki_D] & ksc_DOWN)
		{
			imgpos.x += 10.0f;
			imgObject->SetPos(imgpos);
			//camera->MoveSide(-0.1f);
		}

		if (input.GetKeyState()[vki_LEFT] & ksc_DOWN)
		{
			//camera->TurnRightLeft(2.0f);
		}

		if (input.GetKeyState()[vki_RIGHT] & ksc_DOWN)
		{
			//camera->TurnRightLeft(-2.0f);
		}

		if (input.GetKeyState()[vki_UP] & ksc_DOWN)
		{
			rota += 1.0f;
			imgObject->SetRota(rota);
			//camera->TurnUpDown(2.0f);
		}

		if (input.GetKeyState()[vki_DOWN] & ksc_DOWN)
		{
			rota -= 1.0f;
			imgObject->SetRota(rota);
			//camera->TurnUpDown(-2.0f);
		}

		player->Draw();
		imgObject->Draw();
		
		//imgObject->SetPos(0, 0, 0);
		//imgObject->SetScale(2);

		//camera->SetPos(cPos);

		//model->Draw();

		priMgr.Draw();

		CallEndPerGameLoop();

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			break;
		}

	}
	d12->GetDev()->Release();
	UnregisterClass(APP_CLASS_NAME, hInst);
}
