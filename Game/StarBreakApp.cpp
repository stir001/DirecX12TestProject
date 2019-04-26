#include "StarBreakApp.h"
#include "Dx12MSLib.h"


StarBreakApp::StarBreakApp()
{
}

StarBreakApp::~StarBreakApp()
{
}

void StarBreakApp::Initialize(HINSTANCE hInst)
{
	Dx12CtrlInit(hInst, "StarBreak");
}

void StarBreakApp::Terminate()
{
	Dx12CtrlEnd();
}

void StarBreakApp::Run()
{
	DxInput input;
	while (ProcessMessage() && !input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_ESCAPE))
	{

	}
}
