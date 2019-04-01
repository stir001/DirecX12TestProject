#include "Dx12MSLibCommon.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	Dx12CtrlInit(hInst);
	{
		auto model = LoadFMD("3DModel/FBX/hutyakiti_anim/Hutyakiti_hatON_animeVer.fmd");
		model->SetScale(5.0f);
		DxInput input;
		auto& camera = GetCamera();
		camera->SetTarget(0.0f, 10.0f, 0.0f);
		const float rotaRatio = 1.0f / 3.14159265f * 0.1f;
		while (ProcessMessage()) {
			input.UpdateKeyState();

			if (input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_RIGHT))
			{
				model->AddRotaY(-rotaRatio);
			}
			
			if (input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_LEFT))
			{
				model->AddRotaY(rotaRatio);
			}

			model->Draw();
		}
	}
	Dx12CtrlEnd();
}

