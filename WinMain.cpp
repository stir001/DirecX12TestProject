#include "Dx12MSLib.h"
#include "sample/NormalMapCube.h"

#include <DirectXMath.h>
#include <vector>
#include <map>
#include <memory>
#include <random>

#define BT_NO_SIMD_OPERATOR_OVERLOADS
#include "BulletlibLink.h"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "BulletInclude.h"

#include "sample/NormalMapCube.h"

using namespace DirectX;

const std::string FBX_MODEL_DIR = "3DModel/FBX/";

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	//Direct3D12‚Ì‰Šú‰»
	Dx12CtrlInit(hInst);
	
	auto& camera = Dx12Ctrl::Instance().GetCameraHolder()->GetCamera(0);
	camera->SetPos({ 0, 40, -100 });
	camera->SetTarget({ 0, 30, 0 });

	float cubeLength = 5.0f;
	auto nCube = std::make_shared<NormalMapCube>(cubeLength, "sample/Normalmap/NormalMap.png");
	auto plane = PhysicsSystem::Instance().CreateRigitBody(BulletShapeType::PLANE, { 0,1,0 });
	plane->SetCollisionState(BulletCollisionState::STATIC);

	std::random_device seed_generator;
	std::mt19937 engin(seed_generator());
	

	DxInput input;
	{
		auto& sys = PhysicsSystem::Instance();

		

		while (ProcessMessage()) {
			input.UpdateKeyState();
			camera->DefaultMove(input);
			sys.ClearDebugDraw();

			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_ENTER))
			{
				float range = cubeLength * 2;
				float valueX = (static_cast<float>(engin()) / static_cast<float>(engin._Max + 1) - 0.5f) * range;
				float valueY = (static_cast<float>(engin()) / static_cast<float>(engin._Max + 1) - 0.5f) * range;
				float valueZ = (static_cast<float>(engin()) / static_cast<float>(engin._Max + 1) - 0.5f) * range;
				nCube->Instanceing({valueX, 50 + valueY , valueZ});
			}

			sys.Simulation();
			nCube->AsyncRigidBody();
			nCube->Draw();

			sys.DebugDraw();
		}
	}

	
	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
}
