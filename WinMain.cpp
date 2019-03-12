#include "Dx12MSLib.h"
#include "sample/NormalMapCube.h"
#include "DirectX12/RenderingPass/ShadowmapPass.h"
#include "DirectX12/Rootsignature/PMDBasicShadowRootSignature.h"
#include "DirectX12/Rootsignature/PMDShadowRootSignature.h"
#include "DirectX12/Rootsignature/PMDToonShadowRootSignature.h"
#include "DirectX12/PipelineState/PMDBasicShadowPipelineState.h"
#include "DirectX12/PipelineState/PMDShadowPipelineState.h"
#include "DirectX12/PipelineState/PMDToonShadowPipelineState.h"
#include "DirectX12/Rootsignature/PrimitiveShadowRootSignature.h"
#include "DirectX12/Rootsignature/PrimitiveShadowRenderRootSignature.h"
#include "DirectX12/PipelineState/PrimitiveShadowPipelineState.h"
#include "DirectX12/PipelineState/PrimitiveShadowRenderPipelineState.h"

#include <Windows.h>
#include <DirectXMath.h>
#include <vector>
#include <map>
#include <memory>

#define BT_NO_SIMD_OPERATOR_OVERLOADS
#include "BulletlibLink.h"
#include "BulletInclude.h"

#include <btBulletDynamicsCommon.h>

#include "sample/RigidCube.h"

using namespace DirectX;

const std::string FBX_MODEL_DIR = "3DModel/FBX/";

void SetShadowProperty(std::shared_ptr<PrimitiveController> ctrl, std::shared_ptr<ShadowmapPass> pass);

std::shared_ptr<RigidCube> CreateRigidCube(std::shared_ptr<ShadowmapPass> pass);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	//Direct3D12の初期化
	Dx12CtrlInit(hInst);
	
	auto shadowmapPass = std::make_shared<ShadowmapPass>(Dx12Ctrl::Instance().GetDev());
	ImageLoader::Instance().SetUIPassCommandList(RenderingPassManager::Instance().GetRenderingPassCommandList(DefaultPass::UI));
	ImageLoader::Instance().Set3DPassCommandList(RenderingPassManager::Instance().GetRenderingPassCommandList(DefaultPass::Model));
	ImageLoader::Instance().SetBackGroundPassCommandList(RenderingPassManager::Instance().GetRenderingPassCommandList(DefaultPass::BackGround));
	//FbxLoader::Instance().SetRenderingCommnadList(RenderingPassManager::Instance().GetRenderingPassCommandList(2));
	PrimitiveCreator::Instance().SetRenderingCommandList(RenderingPassManager::Instance().GetRenderingPassCommandList(DefaultPass::Model));
	PhysicsSystem::Instance().SetRenderCommnadList(RenderingPassManager::Instance().GetRenderingPassCommandList(DefaultPass::Model));
	std::shared_ptr<RenderingPassObject> pass = shadowmapPass;
	RenderingPassManager::Instance().InsertRenderingPass(pass, 0);
	auto& camera = Dx12Ctrl::Instance().GetCameraHolder()->GetCamera(0);
	DxInput input;
	{

		auto device = Dx12Ctrl::Instance().GetDev();
		std::shared_ptr<RootSignatureObject> pmdBSRS = std::make_shared<PMDBasicShadowRootSignature>(device);
		std::shared_ptr<RootSignatureObject> pmdSRS = std::make_shared<PMDShadowRootSignature>(device);
		std::shared_ptr<RootSignatureObject> pmdTSRS = std::make_shared<PMDToonShadowRootSignature>(device);
		
		std::shared_ptr<PipelineStateObject> pmdBSPS = std::make_shared<PMDBasicShadowPipelineState>(pmdBSRS, device);
		std::shared_ptr<PipelineStateObject> pmdSPS = std::make_shared<PMDShadowPipelineState>(pmdSRS, device);
		std::shared_ptr<PipelineStateObject> pmdTSPS = std::make_shared<PMDToonShadowPipelineState>(pmdTSRS, device);


		auto img3D = ImageLoader::Instance().LoadImage3D("dice.png" , true);

		PMDLoader loader;
		loader.SetRenderingCommnadList(RenderingPassManager::Instance().GetRenderingPassCommandList(3));
		VMDLoader vmdLoader;
		auto vmd = vmdLoader.LoadAnimation("3DModel/PMD/博麗霊夢/モーション/ヤゴコロダンス.vmd");
		auto model = loader.Load("3DModel/PMD/博麗霊夢/reimu_F01.pmd");
		model->SetAnimation(vmd);
		model->PlayAnimation(true);
		model->SetSkeletonColor(1.0f, 0.0f, 0.0f);
		model->SetShadowmap(shadowmapPass->GetShadowmap());
		model->CreateShadowRenderDescHeap(Dx12Ctrl::Instance().GetDev(),"ShadowmapDescHeap");
		model->SetShadowmapCommandList(RenderingPassManager::Instance().GetRenderingPassCommandList(0));
		model->SetShadowmapPipelineState(pmdSPS);
		model->SetShadowmapRootsignature(pmdSRS);
		model->SetShadowBasicRenderPipelineState(pmdBSPS);
		model->SetShadowBasicRenderRootSignature(pmdBSRS);
		model->SetShadowRenderPipelineState(pmdTSPS);
		model->SetShadowRenderRootsignature(pmdTSRS);
		float scale = 0.01f;

		DirectX::XMFLOAT3 pos = { 0,0,10 };

		auto plane = PrimitiveCreator::Instance().CreatePlane({ 0,0,0 }, 100, 100, { 0,1,0 });
		
		SetShadowProperty(plane, shadowmapPass);

		auto rigidplane = PhysicsSystem::Instance().CreateRigidBody(BulletShapeType::PLANE, { 0,1,0 });
		rigidplane->SetCollisionState(BulletCollisionState::STATIC);
		auto cap = PhysicsSystem::Instance().CreateRigidBody(BulletShapeType::CAPSULE, { 5,10,0 }, { 0,10,0 });
		cap->SetCollisionState(BulletCollisionState::STATIC);

		std::list<std::shared_ptr<RigidCube>> cubes;

		while (ProcessMessage()) {
			input.UpdateKeyState();
			if (input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_Z))
			{
				scale += 0.001f;
			}

			if (input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_X))
			{
				scale -= 0.001f;
			}
			camera->DefaultMove(input);

			PhysicsSystem::Instance().Simulation();

			if (input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_NUMPAD4))
			{
				pos.x -= 0.1f;
			}

			if (input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_NUMPAD6))
			{
				pos.x += 0.1f;
			}

			if (input.IsKeyTrigger(VIRTUAL_KEY_INDEX::KEY_ENTER))
			{
				cubes.push_back(CreateRigidCube(shadowmapPass));
			}

			for (auto cube : cubes)
			{
				cube->AsyncTransform();
				cube->Draw();
			}

			model->DrawShadowmap();
			model->DrawShadow();
			
			plane->DrawShadowmap();
			plane->DrawShadow();
			//PhysicsSystem::Instance().DebugDraw();

		}
	}
	camera = nullptr;
	
	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
}

void SetShadowProperty(std::shared_ptr<PrimitiveController> ctrl, std::shared_ptr<ShadowmapPass> pass)
{
	auto device = Dx12Ctrl::Instance().GetDev();
	std::shared_ptr<RootSignatureObject> prvSRS = std::make_shared<PrimitiveShadowRootSignature>(device);
	std::shared_ptr<RootSignatureObject> prvSRRS = std::make_shared<PrimitiveShadowRenderRootSignature>(device);

	std::shared_ptr<PipelineStateObject> prvSPS = std::make_shared<PrimitiveShadowPipelineState>(prvSRS, device);
	std::shared_ptr<PipelineStateObject> prvSRPS = std::make_shared<PrimitiveShadowRenderPipelineState>(prvSRRS, device);

	ctrl->SetShadowmapCommandList(RenderingPassManager::Instance().GetRenderingPassCommandList(0));
	ctrl->SetShadowmapRootSignature(prvSRRS);
	ctrl->SetShadowmapPipelineState(prvSRPS);
	ctrl->SetShadowRenderRootSignature(prvSRS);
	ctrl->SetShadowRenderPipelineState(prvSPS);
	ctrl->SetShadowmapTexture(pass->GetShadowmap());
	ctrl->CreateShadowmapDescHeap();
}

std::shared_ptr<RigidCube> CreateRigidCube(std::shared_ptr<ShadowmapPass> pass)
{
	auto cube = std::make_shared<RigidCube>();
	SetShadowProperty(cube->GetCtrl(), pass);
	return cube;
}