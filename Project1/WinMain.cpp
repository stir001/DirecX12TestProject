#include "Dx12Ctrl.h"
#include "NeedCall.h"
#include "PMDLoader.h"
#include "PrimitiveCreator.h"
#include "PMDController.h"
#include "VMDLoader.h"
#include "Dx12Camera.h"
#include "DxInput.h"
#include "DirectionalLight.h"
#include "ImageLoader.h"
#include "ImageController.h"
//#include "FbxLoader.h"
#include "PMDController.h"
#include "VMDLoader.h"
#include "PrimitiveController.h"
#include "RenderingPathManager.h"
#include "XMFloatOperators.h"
#include "Transform3DCalculator.h"

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
const std::string PMD_MODEL_PATH = "初音ミク.pmd";
const std::string PMD_MODEL_PATH2 = "博麗霊夢/reimu_F01.pmd";


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	///Direct3D12の初期化

	Dx12Ctrl::Instance().SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	std::string wName = "1601295_真鍋奨一郎";
	Dx12Ctrl::Instance().SetWindowName(wName);
	Dx12Ctrl::Instance().Dx12Init(hInst);

	PrimitiveCreator priCreater;

	float length = 20.f;
	std::shared_ptr<PrimitiveController> primitiveCtrl = priCreater.CreateCubeNormalMap(length, "1024px-Normal_map_example_-_Map.png");

	float deg = -1.0f;

	XMFLOAT3 pos = { 0,0,0 };
	XMFLOAT3 normal = { 0,1,0 };

	Transform3DCalculator calculator;

	std::vector<DirectX::XMFLOAT4X4> instanceMatrix(1);

	auto& camera = Dx12Ctrl::Instance().GetCamera();
	DxInput input;

	DirectX::XMFLOAT3 zVec = { 0, 0, 1 };
	DirectX::XMFLOAT3 xVec = { 1, 0, 0 };
	DirectX::XMFLOAT3 vel = { 0 ,0, 0 };
	float velLength = 2.0f;
	DirectX::XMFLOAT3 pmdPos = {0, 0, 0};
	DirectX::XMFLOAT3 initDir = {0, 0, -1};
	float zvecRota = 90;//-z
	float xvecRota = 180;//-x

	float modelrota = 0;
	unsigned int rotaCount = 0;

	while (ProcessMessage()) {
		CallStartPerGameLoop();
		input.UpdateKeyState();

		camera->DefaultMove(input);

		if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD4))
		{
			calculator.AddRotaY(-deg);
		}

		if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD8))
		{
			calculator.AddRotaX(-deg);
		}

		if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD6))
		{
			calculator.AddRotaY(deg);
		}

		if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD2))
		{
			calculator.AddRotaX(deg);
		}

		instanceMatrix[0] = calculator.GetAMatrix();
		primitiveCtrl->SetInstancingMatrix(instanceMatrix, 0, 0);

		vel = { 0,0,0 };
		modelrota = 0.0f;
		rotaCount = 0;

		primitiveCtrl->Draw();

		CallEndPerGameLoop();
	}

	camera = nullptr;

	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
	
}
