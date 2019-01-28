#include "Dx12MSLib.h"
#include "BulletInclude.h"
#include "sample/NormalMapCube.h"

#include <Windows.h>
#include <DirectXMath.h>
#include <vector>
#include <map>
#include <memory>

using namespace DirectX;

const std::string FBX_MODEL_DIR = "3DModel/FBX/";

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	//Direct3D12‚Ì‰Šú‰»
	Dx12CtrlInit(hInst);
	
	auto& camera = Dx12Ctrl::Instance().GetCameraHolder()->GetCamera(0);
	DxInput input;
	{
		

		auto cap = PrimitiveCreator::Instance().CreateCapsule(5, 10);

		while (ProcessMessage()) {
			input.UpdateKeyState();

			//fbx->Draw();

			cap->Draw();

			camera->DefaultMove(input);
		}

		
	}
	camera = nullptr;
	
	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
}