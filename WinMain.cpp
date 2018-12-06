#include "Dx12MSLib.h"
#include "sample/NormalMapCube.h"

#include <DirectXMath.h>
#include <vector>
#include <map>
#include <memory>
#include <random>

#define BT_NO_SIMD_OPERATOR_OVERLOADS

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "BulletInclude.h"

#include "sample/NormalMapCube.h"
#include "TestAction2.h"

using namespace DirectX;

const std::string FBX_MODEL_DIR = "3DModel/FBX/";

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	//Direct3D12‚Ì‰Šú‰»
	Dx12CtrlInit(hInst);
	{
		auto size = Dx12Ctrl::Instance().GetWindowSize();

		auto& camera = Dx12Ctrl::Instance().GetCameraHolder()->GetCamera(0);
		Dx12Ctrl::Instance().GetCameraHolder()->CreateCamera({ 0,40,100 }, { 0,30,0 }, { 0,0,size.x * 0.5f,size.y * 0.5f,0,1 }, { 0,0,(LONG)(size.x * 0.5), (LONG)(size.y * 0.5) });
		camera->SetPos({ 0, 40, -100 });
		camera->SetTarget({ 0, 30, 0 });

		float cubeLength = 5.0f;

		//auto plane = PhysicsSystem::Instance().CreateRigitBody(BulletShapeType::PLANE, { 0,1,0 });
		//plane->SetCollisionState(BulletCollisionState::STATIC);
		//plane->SetTag(0);

		std::random_device seed_generator;
		std::mt19937 engin(seed_generator());

		auto shape = PhysicsSystem::Instance().CreateCollisionShape(BulletShapeType::BOX, { 1.0f,1.0f,1.0f });
		auto shape2 = PhysicsSystem::Instance().CreateCollisionShape(BulletShapeType::SPHERE, { 1.0f,0,0 });

		DirectX::XMFLOAT3 s2Pos = { 0,0,5 };
		DirectX::XMFLOAT3 s1Pos = { 0,0,1 };
		auto test = std::make_shared<TestAction2>(shape, 1);
		auto test2 = std::make_shared<TestAction2>(shape2, 2);
		auto test3 = std::make_shared<TestAction2>(shape2, 3);
		auto test4 = std::make_shared<TestAction2>(shape2, 4);
		test->Translate(s1Pos);



		

		DxInput input;
		{


			while (ProcessMessage()) {
				input.UpdateKeyState();
				camera->DefaultMove(input);
				PhysicsSystem::Instance().ClearDebugDraw();


				PhysicsSystem::Instance().Simulation();

				if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD8))
				{
					s2Pos.z += 0.1f;
					//test->Remove();
				}

				if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD2))
				{
					s2Pos.z -= 0.1f;
					test = nullptr;
				}

				test2->Translate(s2Pos);

				PhysicsSystem::Instance().DebugDraw();
			}
			
		}
		PhysicsSystem::Destory();
	}
	


	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
}
