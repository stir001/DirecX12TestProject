#include "StarBreakApp.h"
#include "Game/Scene/GameScene/GameScene.h"
#include <Dx12MSLib.h>
#include <Windows.h>


StarBreakApp::StarBreakApp()
{
}

StarBreakApp::~StarBreakApp()
{
}

void StarBreakApp::Initialize(HINSTANCE hInst)
{
#ifndef _DEBUG
	SetShaderDir("Shader/");
#endif
	SetIcon("IDI_ICON1\0");
	Dx12CtrlInit(hInst, "CutCutCut");
	mScene = std::make_shared<GameScene>(GameScene::PrimitiveType::CUBE);
	mScene->Initialize();
	mImg = LoadImage2D("Game/resource/BackBlack.png");
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
		mImg->BackDraw();
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
