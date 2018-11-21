#include "Dx12MSLib.h"
#include "sample/NormalMapCube.h"
#include "PipelineState/BillboardPipelineState.h"
#include "Rootsignature/BillboardRootSignature.h"

#include <Windows.h>
#include <d3d12.h>
#include "d3dx12.h"
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <map>
#include <memory>

#define BT_NO_SIMD_OPERATOR_OVERLOADS
#include "BulletlibLink.h"

#include <btBulletDynamicsCommon.h>
#include "PhysicsSystem.h"

using namespace DirectX;

const std::string FBX_MODEL_DIR = "3DModel/FBX/";

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	//Direct3D12‚Ì‰Šú‰»
	Dx12CtrlInit(hInst);
	

	auto& camera = Dx12Ctrl::Instance().GetCameraHolder()->GetCamera(0);
	
	DxInput input;
	{
		auto sys = new PhysicsSystem();

		auto priCube = PrimitiveCreator::Instance().CreateCube(10.f);
		auto priSph = PrimitiveCreator::Instance().CreateSphere(30.f, 10);
		auto priPlane = PrimitiveCreator::Instance().CreatePlane(DirectX::XMFLOAT3(0.f, 0.f, 0.f), 20.f, 20.f, DirectX::XMFLOAT3(0.f, 1.f, 0.f));

		priSph->SetColor(DirectX::XMFLOAT4(0.5f, 0.f, 0.2f, 1.0f));
		
		while (ProcessMessage()) {
			input.UpdateKeyState();
			camera->DefaultMove(input);

			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_Z))
			{
			
			}

			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_X))
			{
			
			}

			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD4))
			{
				
			}

			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD6))
			{
				
			}

			priSph->Draw();
			priPlane->Draw();
		}
	}

	
	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
}
