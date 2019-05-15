#include "StarBreakApp.h"
#include "BreakSphere.h"
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

	auto star = std::make_shared<BreakSphere>();
	auto ctrl = PrimitiveCreator::Instance().CreateCustumPrimitve(star);

	auto camera = GetCamera();

	auto plane = PrimitiveCreator::Instance().CreatePlane({ 0,0,0 }, 30.0f, 30.0f, { 0.0f,1.0f,1.0f });
	auto plane2 = PrimitiveCreator::Instance().CreatePlane({ 0,0,0 }, 30.0f, 30.0f, { 0.0f,-1.0f,-1.0f });

	while (ProcessMessage() && !input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_ESCAPE))
	{
		input.UpdateKeyState();
		if (input.IsKeyTrigger(VIRTUAL_KEY_INDEX::KEY_ENTER))
		{
			star = std::make_shared<BreakSphere>();
			ctrl = PrimitiveCreator::Instance().CreateCustumPrimitve(star);
		}

		camera->DefaultMove(input);
		ctrl->Draw();
		//plane->Draw();
		//plane2->Draw();
	
	}
}
