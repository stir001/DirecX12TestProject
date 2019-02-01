#include <Dx12MSLib.h>
#include <Windows.h>

#include "MoveRestrictionApp.h"
#include "Collision/CollisionTestApp.h"
#include "HitRepulsionApp.h"
#include "MoveRestriction3DApp.h"

using namespace DirectX;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	MoveRestriction3DApp move3DApp;
	move3DApp.Initialize(hInst);
	move3DApp.Run();
	//HitRepulsionApp throwApp;
	//throwApp.Initialize(hInst);
	//throwApp.Run();
	//MoveRestrictionApp app;
	//app.Initialize(hInst);
	//app.Run();
	//CollisionTestApp colApp;
	//colApp.Initialize(hInst);
	//colApp.Run();
}
