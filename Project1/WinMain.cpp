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
#include "ImageController.h"
#include "FbxLoader.h"
#include "FbxModelDataCoverter.h"
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
using namespace Fbx;

const int WINDOW_WIDTH = 768;
const int WINDOW_HEIGHT = 448;
const TCHAR* APP_CLASS_NAME = _T("DirectX12Test");
const int SCREEN_BUFFER_COUNT = 2;
const std::string FBX_MODEL_PATH = "Model/FBX/test_model/model/test_model.fbx";
const std::string PMD_MODEL_PATH = "初音ミク.pmd";
const std::string PMD_MODEL_PATH2 = "博麗霊夢/reimu_F01.pmd";


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	///Direct3D12の初期化

	Dx12Ctrl* d12 = Dx12Ctrl::Instance();
	d12->SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	std::string wName = "FbxLoad";
	d12->SetWindowName(wName);
	d12->Dx12Init();

	FbxLoader::Create();
	FbxModelData* modelData = FbxLoader::Instance().LoadMesh(FBX_MODEL_PATH);
	FbxModelDataConverter* fbxconverter = new FbxModelDataConverter();
	FbxModel* fbxModel = fbxconverter->ConvertToFbxModel(modelData);

	//PMDLoader loader;
	//PMDController* pmdContrl = loader.Load(PMD_MODEL_PATH2);
	//std::shared_ptr<DirectionalLight> dirLight(new DirectionalLight(1,-1,1));

	//pmdContrl->SetLight(dirLight);
	//メインループ
	while (ProcessMessage()) {
		CallStartPerGameLoop();


		CallEndPerGameLoop();
	}
	d12->Release();
#ifdef _DEBUG
	ID3D12DebugDevice* debugdev = nullptr;
	d12->GetDev()->QueryInterface(&debugdev);
	debugdev->ReportLiveDeviceObjects(D3D12_RLDO_FLAGS::D3D12_RLDO_DETAIL);
#endif
	
}
