#include "Game/StarBreakApp.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	StarBreakApp app;
	app.Initialize(hInst);
	app.Run();
	app.Terminate();
}

