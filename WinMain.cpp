#include "Dx12MSLib.h"
#include "BulletInclude.h"
#include "sample/NormalMapCube.h"

#include <Windows.h>
#include <DirectXMath.h>

using namespace DirectX;

const float INIT_ROTA_Y = 180.0f;
const float INIT_SCALE = 5.0f;
const DirectX::XMFLOAT3 INIT_TARGET = { 0.0f, 10.0f, 0.0f };
const float ROTATE = 1.0f;
const std::string MODEL_PATH = "3DModel/FBX/hutyakiti_anim/Hutyakiti_hatON_animeVer.fmd";

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	Dx12CtrlInit(hInst);

	{
		auto& camera = GetCamera(0);
		camera->SetTarget(INIT_TARGET);
		//auto fbxmodel = FbxLoader::Instance().LoadFMD(MODEL_PATH);
		//fbxmodel->AddRotaY(INIT_ROTA_Y);
		//fbxmodel->SetScale(INIT_SCALE);
		auto image = ImageLoader::Instance().LoadImage2D("dice.png");
		image->SetScale(0.5f);
		DxInput input;
		bool isDraw = false;
		while (ProcessMessage()) {
			input.UpdateKeyState();

			if (input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_NUMPAD4))
			{
				
				//fbxmodel->AddRotaY(ROTATE);
			}

			if (input.IsKeyTrigger(VIRTUAL_KEY_INDEX::KEY_NUMPAD6))
			{
				//fbxmodel->AddRotaY(-ROTATE);
			
				isDraw = true;
			}

			//fbxmodel->Draw();
			image->SetPos({ -100.0f,100.0f,0.0f });
			image->Draw();

			image->SetRect({ 0.0f, 0.0f, 0.0f }, 250.0f, 250.0f);
			image->SetPos({ 100.0f,-100.0f,0.0f });
			image->Draw();
		}

		camera = nullptr;
	}
	
	Dx12CtrlEnd();
}