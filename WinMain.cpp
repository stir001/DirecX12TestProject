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
	//Direct3D12の初期化
	Dx12CtrlInit(hInst);
	
	auto sys = new PhysicsSystem();


	auto& camera = Dx12Ctrl::Instance().GetCameraHolder()->GetCamera(0);
	auto wndsize = Dx12Ctrl::Instance().GetWindowSize();
	//Dx12Ctrl::Instance().GetCameraHolder()->CreateCamera(DirectX::XMFLOAT3(30, 20, 0), DirectX::XMFLOAT3(0, 0, 0), D3D12_VIEWPORT{ wndsize.x * 0.5f,0,wndsize.x * 0.5f,wndsize.y * 0.5f,0.0f,1.0f }, D3D12_RECT{ static_cast<LONG>(wndsize.x * 0.5f), 0, static_cast<LONG>(wndsize.x * 0.5f * 2.0f), static_cast<LONG>(wndsize.y * 0.5f) });
	//Dx12Ctrl::Instance().GetCameraHolder()->CreateCamera(DirectX::XMFLOAT3(0, 20, 30), DirectX::XMFLOAT3(0, 0, 0), D3D12_VIEWPORT{ 0,wndsize.y * 0.5f,wndsize.x * 0.5f,wndsize.y * 0.5f,0.0f,1.0f }, D3D12_RECT{ 0,static_cast<LONG>(wndsize.y * 0.5f), static_cast<LONG>(wndsize.x * 0.5f) , static_cast<LONG>(wndsize.y * 0.5f * 2) });
	//Dx12Ctrl::Instance().GetCameraHolder()->CreateCamera(DirectX::XMFLOAT3(-30, 20, 0), DirectX::XMFLOAT3(0, 0, 0), D3D12_VIEWPORT{ wndsize.x * 0.5f,wndsize.y * 0.5f,wndsize.x * 0.5f,wndsize.y * 0.5f,0.0f,1.0f }, D3D12_RECT{ static_cast<LONG>(wndsize.x * 0.5f) , static_cast<LONG>(wndsize.y * 0.5f), static_cast<LONG>(wndsize.x * 0.5f * 2.0f), static_cast<LONG>(wndsize.y * 0.5f * 2) });
	DxInput input;
	{
		//auto fbxmodel = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "alicia/Alicia_solid_MMD.FBX");//テキストベース
		//auto fbxmodel = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "alicia/Alicia_solid_Unity.FBX");//テキストベース
		//auto fbxmodel = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "chami/tyami_normalVer.fbx");
		//auto fbxmodel = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "chami_anim/tyami_animVer.fbx");
		//auto fbxmodel = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "douki/Douki_chan/Douki_chan.fbx");
		//auto fbxmodel = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "hutyakiti/Hutyakiti_hatON_lowVer.fbx");
		//auto fbxmodel = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "hutyakiti_anim/Hutyakiti_hatON_animeVer.fbx");
		//auto fbxmodel = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "kagura_anim/Kagura_animeVer.fbx");
		//auto fbxmodel = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "kagura_low/kagura_lowVer.fbx");
		//auto fbxmodel = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "kouhai/Kouhai_chan.fbx");
		//auto fbxmodel = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "Senpai_san/Senpai_san.fbx");
		//auto fbxmodel = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "shachiku/ShachikuChan_ver3.fbx");
		//auto fbxmodel = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "yuko_anim/Yuko_animeVer.fbx");
		//auto fbxmodel = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "yuko_low/YukoLowVer.fbx");
		//auto fbxmodel = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "Yuko'sRoom/Yuko'sRoom/Yuko'sRoom.fbx");
		//auto fbxmodel = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "zunko_model_data/TouhokuZunko_FromBlender_20140620_3.fbx");
		//auto fbxmodel = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "CandyRockStar/CandyRockStar.fbx");

		auto img = ImageLoader::Instance().LoadImageData("dice.png");
		auto img3D = ImageLoader::Instance().LoadImage3D("dice.png");
		auto imgBill = ImageLoader::Instance().LoadImage3D("dice.png", true);

		auto priCube = PrimitiveCreator::Instance().CreateCube(10.f, "dice.png");
		auto priSph = PrimitiveCreator::Instance().CreateSphere(30.f, 10, "unity_chan.png");
		auto priPlane = PrimitiveCreator::Instance().CreatePlane(DirectX::XMFLOAT3(0.f, 0.f, 0.f), 20.f, 20.f, DirectX::XMFLOAT3(0.f, 1.f, 0.f));


		//fbxmodel->SetSkeletonColor(1.0f, 0.0f, 0.0f);
		float scale = 0.01f;
		img3D->SetScale(0.01f);
		float fbxScale = 1.0f;

		DirectX::XMFLOAT3 pos = { 0,0,10 };
		//imgBill->SetPosition(pos);
		imgBill->SetScale(scale);
		//img->SetPos(pos);

		
		while (ProcessMessage()) {
			input.UpdateKeyState();
			//fbxmodel->AddRotaY(0.1f);
			//img3D->AddRotaY(0.1f);
			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_Z))
			{
				scale += 0.001f;
				imgBill->SetScale(scale);
				//img->SetScale(scale);
				fbxScale += 0.1f;
				//fbxmodel->SetScale(fbxScale);
			}

			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_X))
			{
				scale -= 0.001f;
				imgBill->SetScale(scale);
				//img->SetScale(scale);
				fbxScale -= 0.1f;
				//fbxmodel->SetScale(fbxScale);
			}
			//camera->DefaultMove(input);


			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD4))
			{
				pos.x -= 0.1f;
			}

			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD6))
			{
				pos.x += 0.1f;
			}

			//fbxmodel->Draw();
			//fbxmodel->DrawSkeleton();

			//img->Draw();
			//imgBill->SetCenterPos(pos);
			//imgBill->Draw();
			//img3D->Draw();
			img->BackDraw();
			//img->BackDraw();
			priCube->AddRotaY(0.1f);
			priCube->Draw();
			priPlane->Draw();
			//priSph->SetPosition(pos);
			//priSph->Draw();

			//camera->SetTarget(pos);
		}
		//delete sys;
	}
	camera = nullptr;

	delete sys;
	
	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
}
