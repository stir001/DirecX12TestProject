#include "StarBreakApp.h"
#include "GameScene.h"
#include <Dx12MSLib.h>


StarBreakApp::StarBreakApp()
{
}

StarBreakApp::~StarBreakApp()
{
}

void StarBreakApp::Initialize(HINSTANCE hInst)
{
	Dx12CtrlInit(hInst, "CutCutCut");
	mScene = std::make_shared<GameScene>(GameScene::PrimitiveType::CUBE);
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
		DemoUpdate(input);
		mScene->Update(input);
		mScene->Draw();
	}
}

void StarBreakApp::DemoUpdate(const DxInput & input)
{
	if (input.IsKeyTrigger(VIRTUAL_KEY_INDEX::KEY_1))
	{
		mScene = std::make_shared<GameScene>(GameScene::PrimitiveType::CUBE);
		mScene->Initialize();
	}
	else if (input.IsKeyTrigger(VIRTUAL_KEY_INDEX::KEY_2))
	{
		mScene = std::make_shared<GameScene>(GameScene::PrimitiveType::SPHERE);
		mScene->Initialize();
	}
	else if (input.IsKeyTrigger(VIRTUAL_KEY_INDEX::KEY_3))
	{
		mScene = std::make_shared<GameScene>(GameScene::PrimitiveType::CAPSULE);
		mScene->Initialize();
	}
	else if (input.IsKeyTrigger(VIRTUAL_KEY_INDEX::KEY_4))
	{
		mScene = std::make_shared<GameScene>(GameScene::PrimitiveType::CONE);
		mScene->Initialize();
	}
}
