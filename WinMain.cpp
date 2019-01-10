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

		auto anim = FbxLoader::Instance().LoadAnimation("0110/chara2Walk.fbx");

		while (ProcessMessage()) {
			input.UpdateKeyState();
		}
	}
	camera = nullptr;
	
	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
}