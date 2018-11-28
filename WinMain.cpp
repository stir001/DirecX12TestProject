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
		auto sphereRigid = sys->CreateRigitBody(BulletShapeType::SPHERE, { 1.0f,0.0f,0.0f }, { 4, 0, 0 });
		//auto capCol = sys->CreateRigitBody(BulletShapeType::CAPSULE, { 1.0f, 2.0f, 0.0f }, { -4,0,0 });
		auto planeRigid = sys->CreateRigitBody(BulletShapeType::PLANE, { 0.0f, 1.f, 0.f });
		auto boxRigid = sys->CreateRigitBody(BulletShapeType::BOX, DirectX::XMFLOAT3( 1.f, 1.f, 1.f), DirectX::XMFLOAT3(0, 0, 0));
		boxRigid->SetCollisionState(BulletCollisionState::CHARACTER);
		//auto cylinderRigid = sys->CreateRigitBody(BulletShapeType::CYLINDER, { 1.0f, 1.0f, 0 }, DirectX::XMFLOAT3(0, 0, 4));
		//auto coneRigid = sys->CreateRigitBody(BulletShapeType::CONE, { 1.0f, 2.0f, 0 });

		//auto priPlane = PrimitiveCreator::Instance().CreatePlane(DirectX::XMFLOAT3(0.f, 0.f, 0.f), 10.f, 10.f, DirectX::XMFLOAT3(0.f, 1.f, 0.f));
		//priPlane->SetColor(DirectX::XMFLOAT4(0.8f,0.5f,0.0f,1.0f));

		


		auto priSphere = PrimitiveCreator::Instance().CreateSphere(1, 10);
		priSphere->SetPosition({ 4.f, 1.f, 0.f });
		float length = 1.0f;
		auto priCube = PrimitiveCreator::Instance().CreateCube(length);
		priCube->SetPosition({ 0, 0 ,0 });

		std::vector<DirectX::XMFLOAT4X4> instanceMat(1);
		instanceMat[0] = boxRigid->GetWorldTransform();
		priCube->SetInstancingMatrix(instanceMat, 0, 0);

		DirectX::XMFLOAT3 pos = { 0,0,0 };

		while (ProcessMessage()) {
			input.UpdateKeyState();
			camera->DefaultMove(input);
			sys->ClearDebugDraw();

			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD6))
			{
				pos.x += 0.1f;
			}

			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD4))
			{
				pos.x -= 0.1f;
			}

			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD8))
			{
				pos.z += 0.1f;
			}

			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD2))
			{
				pos.z -= 0.1f;
			}

			if (input.IsKeyTrigger(eVIRTUAL_KEY_INDEX_RSHIFT))
			{
				boxRigid->SetCollisionState(BulletCollisionState::NON_CONTACT);
			}

			if (input.IsKeyTrigger(eVIRTUAL_KEY_INDEX_RCONTROL))
			{
				boxRigid->SetCollisionState(BulletCollisionState::KINEMATIC);
			}

			priCube->SetPosition(pos);
			boxRigid->SetWorldTransform(priCube->GetMatrix());
			sys->Simulation();
			//instanceMat[0] = boxRigid->GetWorldTransform();
			//priCube->SetInstancingMatrix(instanceMat, 0, 0);

			//priSphere->Draw();
			priCube->Draw();
			sys->DebugDraw();

			
		}

		//sys.reset();
	}

	
	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
}
