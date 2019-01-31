#include <Dx12MSLib.h>
#include <Windows.h>

#include "MoveRestrictionApp.h"
#include "Collision/CollisionTestApp.h"

using namespace DirectX;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	MoveRestrictionApp app;
	app.Initialize(hInst);
	app.Run();
	//CollisionTestApp colApp;
	//colApp.Initialize(hInst);
	//colApp.Run();
}
