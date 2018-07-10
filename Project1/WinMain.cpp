#include "Dx12Ctrl.h"
#include "NeedCall.h"
#include "PMDLoader.h"
#include "PrimitiveManager.h"
#include "PMDController.h"
#include "VMDLoader.h"
#include "Dx12Camera.h"
#include "DxInput.h"
#include "DirectionalLight.h"
#include "ImageLoader.h"
#include "ImageController.h"
//#include "FbxLoader.h"
#include "PMDController.h"

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
#include <memory>

using namespace DirectX;

const int WINDOW_WIDTH = 768;
const int WINDOW_HEIGHT = 448;
const TCHAR* APP_CLASS_NAME = _T("DirectX12Test");
const int SCREEN_BUFFER_COUNT = 2;
const std::string FBX_MODEL_PATH = "Model/FBX/test_model/model/test_model.fbx";
const std::string PMD_MODEL_PATH = "‰‰¹ƒ~ƒN.pmd";
const std::string PMD_MODEL_PATH2 = "”Ž—í—ì–²/reimu_F01.pmd";
const std::string FBX_MODEL_PATH2 = "test_moedl2/FBX/test_model_low.FBX";
const std::string FBX_MODEL_PATH3 = "test_moedl2/FBX/test/test_model_low.FBX";


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	///Direct3D12‚Ì‰Šú‰»

	/*Dx12Ctrl& d12 = Dx12Ctrl::Instance();*/
	Dx12Ctrl::Instance()->SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	std::string wName = "FbxLoad";
	Dx12Ctrl::Instance()->SetWindowName(wName);
	Dx12Ctrl::Instance()->Dx12Init(hInst);

	//FbxLoader::Create();
	//auto modelData = FbxLoader::Instance()->LoadMesh(FBX_MODEL_PATH2);

	//PrimitiveManager mgr;


	PMDLoader loader;
	std::shared_ptr<PMDController> pmdContrl = loader.Load(PMD_MODEL_PATH2);
	std::shared_ptr<DirectionalLight> dirLight(new DirectionalLight(1,-1,1));

	pmdContrl->SetLight(dirLight);



	XMFLOAT3 pos = { 0,0,0 };
	XMFLOAT3 normal = { 0,1,0 };
	//mgr.CreatePlane(pos, 50, 50, normal);
	//mgr.SetLightObject(dirLight);

	auto camera = Dx12Ctrl::Instance()->GetCamera();
	DxInput input;

	while (ProcessMessage()) {
		CallStartPerGameLoop();
		input.UpdateKeyState();

		camera->DefaultMove(input);

		/*pmdContrl->Draw();*/
		//mgr.Draw();

		CallEndPerGameLoop();
	}
	Dx12Ctrl::Instance()->Release();
	Dx12Ctrl::Destroy();
	
}
