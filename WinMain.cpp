#include "Dx12MSLib.h"
#include "sample/NormalMapCube.h"
#include "PipelineState/BillboardPipelineState.h"
#include "Rootsignature/BillboardRootSignature.h"

#include <DirectXMath.h>
#include <vector>
#include <map>
#include <memory>

#define BT_NO_SIMD_OPERATOR_OVERLOADS
#include "BulletlibLink.h"

#include <btBulletDynamicsCommon.h>
#include "bullet/System/PhysicsSystem.h"
#include "bullet/System/BulletDebugDrawDx.h"
#include "bullet/RigidBody/SphereRigidBody.h"
#include "bullet/RigidBody/CapsuleRigidBody.h"
#include "bullet/RigidBody/PlaneRigidBody.h"
#include "bullet/RigidBody/BoxRigidBody.h"

using namespace DirectX;

const std::string FBX_MODEL_DIR = "3DModel/FBX/";

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	//Direct3D12‚Ì‰Šú‰»
	Dx12CtrlInit(hInst);
	
	auto& camera = Dx12Ctrl::Instance().GetCameraHolder()->GetCamera(0);
	
	DxInput input;
	{
		auto sys = std::make_shared<PhysicsSystem>();
		auto colider = std::make_shared<SphereRigidBody>(1.0f, DirectX::XMFLOAT3(4,0,0));
		auto capCol = std::make_shared<CapsuleRigidBody>(1.0f, 3.0f, DirectX::XMFLOAT3(-4, 0, 0));
		auto planeRigid = std::make_shared<PlaneRigidBody>(1.0f);
		auto boxRigid = std::make_shared<BoxRigidBody>(1.f, DirectX::XMFLOAT3(0, 0, -4));

		sys->AddRigidBody(colider);
		sys->AddRigidBody(capCol);
		sys->AddRigidBody(planeRigid);
		sys->AddRigidBody(boxRigid);

		auto priPlane = PrimitiveCreator::Instance().CreatePlane(DirectX::XMFLOAT3(0.f, 0.f, 0.f), 20.f, 20.f, DirectX::XMFLOAT3(0.f, 1.f, 0.f));
		priPlane->SetColor(DirectX::XMFLOAT4(0.8f,0.5f,0.0f,1.0f));
		
		while (ProcessMessage()) {
			input.UpdateKeyState();
			camera->DefaultMove(input);
			sys->ClearDebugDraw();
			sys->Simulation();

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

			sys->DebugDraw();

			//priPlane->Draw();
		}
		sys->RemoveRigidBody(colider);
		sys->RemoveRigidBody(capCol);
		sys->RemoveRigidBody(planeRigid);
		sys->RemoveRigidBody(boxRigid);
		//sys.reset();
	}

	
	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
}
