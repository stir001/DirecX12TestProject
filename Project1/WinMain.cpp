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
const std::string FBX_MODEL_PATH2 = "test_moedl2/FBX/test_model_low.FBX";
const std::string FBX_MODEL_PATH3 = "test_moedl2/FBX/test/test_model_low.FBX";


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	///Direct3D12の初期化

	Dx12Ctrl::Instance().SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	std::string wName = "1601295_真鍋奨一郎";
	Dx12Ctrl::Instance().SetWindowName(wName);
	Dx12Ctrl::Instance().Dx12Init(hInst);

	//FbxLoader::Create();
	//auto modelData = FbxLoader::Instance()->LoadMesh(FBX_MODEL_PATH2);

	//PrimitiveCreator mgr;


	std::shared_ptr<PMDLoader> loader(new PMDLoader());
	std::shared_ptr<PMDController> pmdContrl = loader->Load(PMD_MODEL_PATH2);
	std::shared_ptr<DirectionalLight> dirLight(new DirectionalLight(1, -1, 1));
	VMDLoader vmdloader;
	std::shared_ptr<VMDMotion> motion = vmdloader.LoadMotion("博麗霊夢/reimu_walk.vmd");
	pmdContrl->SetMotion(motion);
	pmdContrl->PlayMotion(true);

	PrimitiveCreator priCreater;

	float length = 20.f;
	std::shared_ptr<PrimitiveController> primitiveCtrl = priCreater.CreateCube(length, "cube_tex.png");

	unsigned int xNum = 100;
	unsigned int zNum = 100;
	unsigned int yNum = 10;
	std::vector<DirectX::XMFLOAT3> offsets(xNum * zNum * yNum);
	for (int y = 0; y < yNum; ++y)
	{
		for (int z = 0; z < zNum; ++z)
		{
			for (int x = 0; x < xNum; ++x)
			{
				offsets[x + z * xNum + y * xNum * zNum] = DirectX::XMFLOAT3(x * length, -y * length, z * length);
			}
		}
	}
	float deg = -1.0f;

	primitiveCtrl->Instancing(offsets);

	pmdContrl->SetLight(dirLight);

	XMFLOAT3 pos = { 0,0,0 };
	XMFLOAT3 normal = { 0,1,0 };

	Transform3DCalculator calculator;

	XMFLOAT4X4 matrix = StoreMatrixToXMFloat4(DirectX::XMMatrixIdentity());

	std::vector<DirectX::XMFLOAT4X4> instanceMatrix(xNum * zNum);


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

		calculator.AddPositon(DirectX::XMFLOAT3(0, length * 0.5f, length * 0.5f));
		matrix *= calculator.GetAMatrix();
		calculator.Init();
		calculator.SetRotaQuaternion(CreateQuoternionXMFloat4(DirectX::XMFLOAT3(1, 0, 0), deg));
		matrix *= calculator.GetAMatrix();
		calculator.Init();
		calculator.AddPositon(DirectX::XMFLOAT3(0, -length * 0.5f, -length * 0.5f));
		matrix *= calculator.GetAMatrix();
		calculator.Init();
		for (auto& mat : instanceMatrix)
		{
			mat = matrix;
		}
		primitiveCtrl->SetInstancingMatrix(instanceMatrix,0, xNum * zNum);

		vel = { 0,0,0 };
		modelrota = 0.0f;
		rotaCount = 0;

		//if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD4) && input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD8))
		//{
		//	modelrota = 135;
		//}

		if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD4))
		{
			vel += -xVec;
		}

		if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD8))
		{
			vel += zVec;
		}

		if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD6))
		{
			vel += xVec;

		}

		if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD2))
		{
			vel += -zVec;
		}

		if (vel != DirectX::XMFLOAT3(0, 0, 0))
		{
			vel = NormalizeXMFloat3(vel);
			pmdPos += vel * velLength;
			
			pmdContrl->SetRotaQuaternion(CreateQuoternionXMFloat4(DirectX::XMFLOAT3(0, 1, 0), modelrota));
		}
		
		pmdContrl->SetPositon(pmdPos);
		pmdContrl->Draw();

		primitiveCtrl->Draw();

		CallEndPerGameLoop();
	}

	camera = nullptr;
	dirLight = nullptr;
	pmdContrl = nullptr;
	loader = nullptr;

	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
	
}
