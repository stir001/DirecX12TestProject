#include "Dx12MSLibCommon.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	Dx12CtrlInit(hInst);
	{
		DxInput input;
		auto& camera = GetCamera(0);
		while (ProcessMessage()) {
			input.UpdateKeyState();

			if (input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_NUMPAD8))
			{
				
			}
			
			if (input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_NUMPAD2))
			{
				
			}

			if (input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_NUMPAD4))
			{
				
			}

			if (input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_NUMPAD6))
			{
				
			}
			camera->DefaultMove(input);
		}
		camera = nullptr;
	}

	Dx12CtrlEnd();
}