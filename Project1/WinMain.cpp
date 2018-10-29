#include "Dx12MSLib.h"
#include "NormalMapCube.h"

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

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	//Direct3D12の初期化
	Dx12CtrlInit(hInst);

	auto& camera = Dx12Ctrl::Instance().GetCamera();
	DxInput input;
	{
		//auto fbxmodel = FbxLoader::Instance().LoadMesh("Senpai_san/Senpai_san.fbx");
		//auto fbxmodel = FbxLoader::Instance().LoadMesh("kouhai/Kouhai_chan.fbx");
		//auto fbxmodel = FbxLoader::Instance().LoadMesh("alicia/Alicia_solid_MMD.FBX");//テキストベース
		//auto fbxmodel = FbxLoader::Instance().LoadMesh("alicia/Alicia_solid_Unity.FBX");//テキストベース
		//auto fbxmodel = FbxLoader::Instance().LoadMesh("shachiku/ShachikuChan_ver3.fbx");
		//auto fbxmodel = FbxLoader::Instance().LoadMesh("hutyakiti/Hutyakiti_hatON_lowVer.fbx");
		auto fbxmodel = FbxLoader::Instance().LoadMesh("hutyakiti_anim/Hutyakiti_hatON_animeVer.fbx");
		//auto fbxmodel = FbxLoader::Instance().LoadMesh("douki/Douki_chan/Douki_chan.fbx");
		//auto fbxmodel = FbxLoader::Instance().LoadMesh("Yuko'sRoom/Yuko'sRoom/Yuko'sRoom.fbx");
		float scale = 1.0f;
		while (ProcessMessage()) {
			input.UpdateKeyState();
			fbxmodel->AddRotaY(0.1f);
			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_Z))
			{
				scale += 0.1f;
				fbxmodel->SetScale(scale);
			}

			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_X))
			{
				scale -= 0.1f;
				fbxmodel->SetScale(scale);
			}
			camera->DefaultMove(input);
			fbxmodel->Draw();
		}
	}
	camera = nullptr;

	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
}
