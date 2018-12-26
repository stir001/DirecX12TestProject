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
//#include "BulletlibLink.h"
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
	std::shared_ptr<RenderingPassObject> pass = shadowmapPass;
	RenderingPassManager::Instance().InsertRenderingPass(pass, 0);
	ImageLoader::Instance().SetUIPassCommandList(RenderingPassManager::Instance().GetRenderingPassCommandList(1));
	ImageLoader::Instance().Set3DPassCommandList(RenderingPassManager::Instance().GetRenderingPassCommandList(2));
	ImageLoader::Instance().SetBackGroundPassCommandList(RenderingPassManager::Instance().GetRenderingPassCommandList(3));
	FbxLoader::Instance().SetRenderingCommnadList(RenderingPassManager::Instance().GetRenderingPassCommandList(2));
	PrimitiveCreator::Instance().SetRenderingCommandList(RenderingPassManager::Instance().GetRenderingPassCommandList(2));
	PhysicsSystem::Instance().SetRenderCommnadList(RenderingPassManager::Instance().GetRenderingPassCommandList(2));
	auto& camera = Dx12Ctrl::Instance().GetCameraHolder()->GetCamera(0);
	DxInput input;
	{
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "alicia/Alicia_solid_MMD.FBX");//テキストベース
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "alicia/Alicia_solid_Unity.FBX");//テキストベース
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "chami/tyami_normalVer.fbx");
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "chami_anim/tyami_animVer.fbx");
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "douki/Douki_chan/Douki_chan.fbx");
		//auto fbxmodel = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "hutyakiti/Hutyakiti_hatON_lowVer.fbx");
		//auto fbxmodel = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "hutyakiti_anim/Hutyakiti_hatON_animeVer.fbx");
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "kagura_anim/Kagura_animeVer.fbx");
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "kagura_low/kagura_lowVer.fbx");
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "kouhai/Kouhai_chan.fbx");
		//auto fbxmodel = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "Senpai_san/Senpai_san.fbx");
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "shachiku/ShachikuChan_ver3.fbx");
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "yuko_anim/Yuko_animeVer.fbx");
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "yuko_low/YukoLowVer.fbx");
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "Yuko'sRoom/Yuko'sRoom/Yuko'sRoom.fbx");
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "zunko_model_data/TouhokuZunko_FromBlender_20140620_3.fbx");
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "CandyRockStar/CandyRockStar.fbx");

		auto device = Dx12Ctrl::Instance().GetDev();
		std::shared_ptr<RootSignatureObject> pmdBSRS = std::make_shared<PMDBasicShadowRootSignature>(device);
		std::shared_ptr<RootSignatureObject> pmdSRS = std::make_shared<PMDShadowRootSignature>(device);
		std::shared_ptr<RootSignatureObject> pmdTSRS = std::make_shared<PMDToonShadowRootSignature>(device);
		
		std::shared_ptr<PipelineStateObject> pmdBSPS = std::make_shared<PMDBasicShadowPipelineState>(pmdBSRS, device);
		std::shared_ptr<PipelineStateObject> pmdSPS = std::make_shared<PMDShadowPipelineState>(pmdSRS, device);
		std::shared_ptr<PipelineStateObject> pmdTSPS = std::make_shared<PMDToonShadowPipelineState>(pmdTSRS, device);


		auto img3D = ImageLoader::Instance().LoadImage3D("dice.png" , true);

		PMDLoader loader;
		loader.SetRenderingCommnadList(RenderingPassManager::Instance().GetRenderingPassCommandList(2));
		VMDLoader vmdLoader;
		auto vmd = vmdLoader.LoadMotion("3DModel/PMD/博麗霊夢/モーション/ヤゴコロダンス.vmd");
		auto model = loader.Load("3DModel/PMD/博麗霊夢/reimu_F01.pmd");
		model->SetMotion(vmd);
		model->PlayMotion(true);
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

		float fbxScale = 1.0f;

		DirectX::XMFLOAT3 pos = { 0,0,10 };

		auto plane = PrimitiveCreator::Instance().CreatePlane({ 0,0,0 }, 100, 100, { 0,1,0 });
		
		SetShadowProperty(plane, shadowmapPass);

		auto rigidplane = PhysicsSystem::Instance().CreateRigidBody(BulletShapeType::PLANE, { 0,1,0 });
		rigidplane->SetCollisionState(BulletCollisionState::STATIC);
		auto cap = PhysicsSystem::Instance().CreateRigidBody(BulletShapeType::CAPSULE, { 5,10,0 }, { 0,10,0 });
		cap->SetCollisionState(BulletCollisionState::STATIC);

		std::list<std::shared_ptr<RigidCube>> cubes;

		float friction = 1.0f;

		auto primitive = PrimitiveCreator::Instance().CreateCube(10);

		auto quaternion = CreateQuoternionXMFloat4({ 1,0,0 }, 60);

		

		while (ProcessMessage()) {
			input.UpdateKeyState();
			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_Z))
			{
				scale += 0.001f;
				fbxScale += 0.1f;
				//model->SetScale(fbxScale);
			}

			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_X))
			{
				scale -= 0.001f;

				fbxScale -= 0.1f;
				//model->SetScale(fbxScale);
			}
			camera->DefaultMove(input);

			PhysicsSystem::Instance().Simulation();

			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD4))
			{
				pos.x -= 0.1f;
			}

			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD6))
			{
				pos.x += 0.1f;
			}

			//if (input.IsKeyTrigger(eVIRTUAL_KEY_INDEX_ENTER))
			//{
			//	cubes.push_back(CreateRigidCube(shadowmapPass));
			//}

			if (input.IsKeyTrigger(eVIRTUAL_KEY_INDEX_K))
			{
				friction = 0.0f;
			}
			else if (input.IsKeyTrigger(eVIRTUAL_KEY_INDEX_J))
			{
				friction = 1.0f;
			}

			//for (auto cube : cubes)
			//{
			//	cube->SetFriction(friction);
			//	cube->AsyncTransform();
			//	cube->Draw();
			//}
			//img3D->AddRotaY(1.f);

			//model->Draw();
			model->DrawShadowmap();
			model->DrawShadow();
			//model->DrawSkeleton();
			//img3D->Draw();
			//fbxmodel->Draw();
			
			plane->DrawShadowmap();
			plane->DrawShadow();
			PhysicsSystem::Instance().DebugDraw();

			primitive->Draw();

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