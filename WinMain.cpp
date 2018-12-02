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
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "bullet/System/PhysicsSystem.h"
#include "bullet/RigidBody/BulletRigidBody.h"
#include "bullet/Action/TestAction.h"
#include "bullet/Ghost/BulletGhostObject.h"


using namespace DirectX;

const std::string FBX_MODEL_DIR = "3DModel/FBX/";

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	//Direct3D12‚Ì‰Šú‰»
	Dx12CtrlInit(hInst);
	
	auto& camera = Dx12Ctrl::Instance().GetCameraHolder()->GetCamera(0);
	
	//camera->SetPos({ 10, 5, 0 });
	//DirectX::XMFLOAT3 targetPos(0,5,0);
	//camera->SetTarget(targetPos);

	PMDLoader loader;
	auto ctrl = loader.Load("3DModel/PMD/”Ž—í—ì–²/reimu_F01.pmd");

	DxInput input;
	{
		auto& sys = PhysicsSystem::Instance();

		while (ProcessMessage()) {
			input.UpdateKeyState();
			camera->DefaultMove(input);
			sys.ClearDebugDraw();

			ctrl->Draw();

			sys.DebugDraw();
		}
	}

	
	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
}
