#include "Dx12MSLib.h"
#include "BulletInclude.h"
#include "sample/NormalMapCube.h"

#include <Windows.h>
#include <DirectXMath.h>

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
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "hutyakiti/Hutyakiti_hatON_lowVer.fbx");
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "hutyakiti_anim/Hutyakiti_hatON_animeVer.fbx");
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "kagura_anim/Kagura_animeVer.fbx");
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "kagura_low/kagura_lowVer.fbx");
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "kouhai/Kouhai_chan.fbx");
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "Senpai_san/Senpai_san.fbx");
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "shachiku/ShachikuChan_ver3.fbx");
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "yuko_anim/Yuko_animeVer.fbx");
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "yuko_low/YukoLowVer.fbx");
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "Yuko'sRoom/Yuko'sRoom/Yuko'sRoom.fbx");
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "zunko_model_data/TouhokuZunko_FromBlender_20140620_3.fbx");
		//auto model = FbxLoader::Instance().LoadMesh(FBX_MODEL_DIR + "CandyRockStar/CandyRockStar.fbx");


		//auto rightWall = sys.CreateRigidBody(BulletShapeType::PLANE, { -1,0,0 });
		//rightWall->SetCollisionState(BulletCollisionState::STATIC);
		//sys.AddRigidBody(rightWall);
		//rightWall->SetOrigin({ 20,0,0 });

		//auto leftWall = sys.CreateRigidBody(BulletShapeType::PLANE, { 1,0,0 });
		//sys.AddRigidBody(leftWall);
		//leftWall->SetOrigin({ -20,0,0 });
		//leftWall->SetCollisionState(BulletCollisionState::STATIC);

		/*auto upWall = sys.CreateRigidBody(BulletShapeType::PLANE, { 0,-1,0 });
		upWall->SetCollisionState(BulletCollisionState::STATIC);
		sys.AddRigidBody(upWall);
		upWall->SetOrigin({ 0,40,0 });*/

		//auto backWall = sys.CreateRigidBody(BulletShapeType::PLANE, { 0,0,1 });
		//sys.AddRigidBody(backWall);
		//backWall->SetOrigin({ 0,0,-20 });
		//backWall->SetCollisionState(BulletCollisionState::STATIC);

		//auto frontWall = sys.CreateRigidBody(BulletShapeType::PLANE, { 0,0,-1 });
		//sys.AddRigidBody(frontWall);
		//frontWall->SetOrigin({ 0,0,20 });
		//frontWall->SetCollisionState(BulletCollisionState::STATIC);

		//auto fbx = FbxLoader::Instance().LoadMesh("0123/chara_1/chara_1.fbx");
		//auto fbx = FbxLoader::Instance().LoadMesh("0123/chara_2/chara_2.fbx");
		//auto fbx = FbxLoader::Instance().LoadFMD("0123/chara_3/chara_3.fmd");
		//auto fbx = FbxLoader::Instance().LoadMesh("0123/chara_4/chara_4.fbx");
		//auto fbx = FbxLoader::Instance().LoadMesh("0122_19/chara.fbx");
		//auto anim = FbxLoader::Instance().LoadAnimation("0123/Stand.fbx");

		//fbx->SetMotion(anim);

		auto cap = PrimitiveCreator::Instance().CreateCapsule(5, 10);

		while (ProcessMessage()) {
			input.UpdateKeyState();

			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD8))
			{
				
			}
			
			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD2))
			{
				
			}

			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD4))
			{
				
			}

			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD6))
			{
				
			}


			camera->DefaultMove(input);
		}

		//sys.RemoveRigidBody(ground);
		//sys.RemoveRigidBody(rightWall);
		//sys.RemoveRigidBody(leftWall);
		//sys.RemoveRigidBody(upWall);
		//sys.RemoveRigidBody(backWall);
		//sys.RemoveRigidBody(frontWall);
	}
	camera = nullptr;
	
	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
}