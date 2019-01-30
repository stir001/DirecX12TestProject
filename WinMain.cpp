#include <Dx12MSLib.h>
#include <Windows.h>
#include <vector>

#include "Collision/CollisionTestApp.h"

using namespace DirectX;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	CollisionTestApp app;
	app.Initialize(hInst);
	app.Run();
}
