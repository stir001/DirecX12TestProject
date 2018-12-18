#include "Dx12MSLib.h"
#include "sample/NormalMapCube.h"

#include <Windows.h>
#include <DirectXMath.h>
#include <vector>
#include <map>
#include <memory>

#define BT_NO_SIMD_OPERATOR_OVERLOADS
#include "BulletlibLink.h"

#include <btBulletDynamicsCommon.h>

using namespace DirectX;

const std::string FBX_MODEL_DIR = "3DModel/FBX/";

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	//Direct3D12の初期化
	Dx12CtrlInit(hInst);

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
		
		auto img3D = ImageLoader::Instance().LoadImage3D("dice.png" , true);

		PMDLoader loader;
		VMDLoader vmdLoader;
		auto vmd = vmdLoader.LoadMotion("3DModel/PMD/博麗霊夢/モーション/ヤゴコロダンス.vmd");
		auto model = loader.Load("3DModel/PMD/博麗霊夢/reimu_F01.pmd");
		model->SetMotion(vmd);
		model->PlayMotion(true);
		model->SetSkeletonColor(1.0f, 0.0f, 0.0f);
		float scale = 0.01f;

		float fbxScale = 1.0f;

		DirectX::XMFLOAT3 pos = { 0,0,10 };

		auto plane = PrimitiveCreator::Instance().CreatePlane({ 0,0,0 }, 100, 100, { 0,1,0 });
		
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


			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD4))
			{
				pos.x -= 0.1f;
			}

			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD6))
			{
				pos.x += 0.1f;
			}

			//img3D->AddRotaY(1.f);

			model->Draw();
			//model->DrawSkeleton();
			//img3D->Draw();
			//fbxmodel->Draw();
			plane->Draw();

		}
	}
	camera = nullptr;
	
	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
}
