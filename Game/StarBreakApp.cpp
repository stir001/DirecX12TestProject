#include "StarBreakApp.h"
#include "GameScene.h"
#include "Dx12MSLib.h"


StarBreakApp::StarBreakApp()
{
}

StarBreakApp::~StarBreakApp()
{
}

void StarBreakApp::Initialize(HINSTANCE hInst)
{
	Dx12CtrlInit(hInst, "CutCutCut");
	mScene = std::make_shared<GameScene>(GameScene::PrimitiveType::SPHERE);
	mScene->Initialize();
}

void StarBreakApp::Terminate()
{
	mScene->Terminate();
	mScene.reset();
	Dx12CtrlEnd();
}

void StarBreakApp::Run()
{
	DxInput input;

	while (ProcessMessage() && !input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_ESCAPE))
	{
		input.UpdateKeyState();
		mScene->Update(input);
		mScene->Draw();
	}
}
