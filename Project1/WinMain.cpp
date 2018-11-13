#include "Dx12MSLib.h"
#include "NormalMapCube.h"
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
#include "btBulletDynamicsCommon.h"
#include "BulletlibLink.h"

using namespace DirectX;

const std::string FBX_MODEL_DIR = "3DModel/FBX/";

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	//Direct3D12の初期化
	Dx12CtrlInit(hInst);

	auto& camera = Dx12Ctrl::Instance().GetCamera();
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

		//auto img = ImageLoader::Instance().LoadImageData("dice.png");
		auto img3d = ImageLoader::Instance().LoadImage3D("dice.png", true);

		auto pri = PrimitiveCreator::Instance().CreateCube(10.f, "dice.png");


		//fbxmodel->SetSkeletonColor(1.0f, 0.0f, 0.0f);
		float scale = 1.0f;

		DirectX::XMFLOAT3 pos = { 0,0,10 };
		img3d->SetCenterPos(pos);
		img3d->SetScale(0.01f);
		//img->SetPos(pos);
		while (ProcessMessage()) {
			input.UpdateKeyState();
			//fbxmodel->AddRotaY(0.1f);
			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_Z))
			{
				scale += 0.1f;
				img3d->SetScale(scale);
				//img->SetScale(scale);
			}

			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_X))
			{
				scale -= 0.1f;
				img3d->SetScale(scale);
				//img->SetScale(scale);
			}
			camera->DefaultMove(input);


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
			img3d->SetCenterPos(pos);
			img3d->Draw();
			//img->Draw();
			pri->Draw();

			//camera->SetTarget(pos);
		}
	}
	camera = nullptr;

	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
}
