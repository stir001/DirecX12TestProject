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


const std::string FBX_MODEL_PATH = "Model/FBX/test_model/model/test_model.fbx";
const std::string PMD_MODEL_PATH = "èââπÉ~ÉN.pmd";
const std::string PMD_MODEL_PATH2 = "îéóÌóÏñ≤/reimu_F01.pmd";


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	///Direct3D12ÇÃèâä˙âª

	Dx12CtrlInit(hInst);

	PrimitiveCreator priCreater;

	float length = 20.f;
	std::shared_ptr<PrimitiveController> primitiveCtrl = priCreater.CreateCubeNormalMap(length, "NormalMap.png");

	auto t_light = std::shared_ptr<LightObject>(new DirectionalLight(1.0f, -0.3f, 0.5f));

	primitiveCtrl->SetLightBuffer(t_light);

	float deg = -1.0f;

	XMFLOAT3 pos = { 0,0,0 };
	XMFLOAT3 normal = { 0,1,0 };

	Transform3DCalculator calculator;

	const unsigned int instanceNum = 10U;
	std::vector<DirectX::XMFLOAT4X4> instanceMatrix(instanceNum);
	for (auto& mat : instanceMatrix)
	{
		mat = StoreMatrixToXMFloat4(DirectX::XMMatrixIdentity());
	}

	std::vector<DirectX::XMFLOAT3> instanceOffsets(instanceNum);
	float offsetX = 0.0f;
	for (auto& offset : instanceOffsets)
	{
		offset = DirectX::XMFLOAT3(offsetX, 0, 0);
		offsetX += length;
	}

	primitiveCtrl->Instancing(instanceOffsets);
	primitiveCtrl->SetInstancingMatrix(instanceMatrix,0 , static_cast<unsigned int>(instanceMatrix.size() - 1));

	auto& camera = Dx12Ctrl::Instance().GetCamera();
	DxInput input;

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

		primitiveCtrl->Draw();

		CallEndPerGameLoop();
	}

	camera = nullptr;

	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
}
