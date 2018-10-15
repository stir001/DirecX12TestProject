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
	//Direct3D12‚Ì‰Šú‰»
	Dx12CtrlInit(hInst);

	ShaderCompiler::Instance().SetShaderModel("5_0");
	auto shaders = ShaderCompiler::Instance().CompileShader(SHADERDIR_PATH + "InterfacesTest.hlsl", "vs", "ps", "", "", "", false);

	ID3D12ShaderReflection* reflection;
	D3DReflect(shaders.pixelShader->GetBufferPointer(), shaders.pixelShader->GetBufferSize(), IID_PPV_ARGS(&reflection));
	auto numSlots = reflection->GetNumInterfaceSlots();

	auto variable = reflection->GetVariableByName("g_lightingClass");
	auto offset = variable->GetInterfaceSlot(0);

	auto& camera = Dx12Ctrl::Instance().GetCamera();
	DxInput input;

	auto fbxmodel = FbxLoader::Instance().LoadMesh("Senpai_san/Senpai_san.fbx");

	while (ProcessMessage()) {
		input.UpdateKeyState();
		fbxmodel->AddRotaY(0.1f);
		camera->DefaultMove(input);
		fbxmodel->Draw();
	}

	camera = nullptr;

	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
}
