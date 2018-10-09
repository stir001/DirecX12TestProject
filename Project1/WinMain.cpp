#include "Dx12MSLib.h"
#include "NormalMapCube.h"

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
	//Direct3D12‚Ì‰Šú‰»
	Dx12CtrlInit(hInst);

	auto& camera = Dx12Ctrl::Instance().GetCamera();
	DxInput input;

	auto fbxmodel = FbxLoader::Instance().LoadMesh("Senpai_san/Senpai_san.fbx");

	while (ProcessMessage()) {
		input.UpdateKeyState();
		fbxmodel->AddRotaY(0.1f);
		camera->DefaultMove(input);
		fbxmodel->Draw();
	}

	camera = nullptr;

	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
}
