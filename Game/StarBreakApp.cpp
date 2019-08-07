#include "StarBreakApp.h"
#include "Game/Scene/GameScene/GameScene.h"
#include "Game/Scene/FreeCutScene/FreeCutScene.h"
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
	auto light = std::make_shared<DirectionalLight>(0.7f, -2.0f, 0.5f);
	PrimitiveCreator::Instance().SetLightObject(light);
	mScene = std::make_shared<GameScene>(GameScene::PrimitiveType::CUBE);
	mScene->Initialize();
	InitImages();
	mUpdateState = &StarBreakApp::GameDemoUpdate;
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
		(this->*mUpdateState)(input);
		mScene->Update(input);
		//mImg->BackDraw();
		mScene->Draw();
		DemoDraw();
	}
}

void StarBreakApp::DemoDraw()
{
	mCube->Draw();
	mSphere->Draw();
	mCapsule->Draw();
	mCone->Draw();
	mModeIcon->Draw();
}

const float SCALE = 0.5f;

void StarBreakApp::GameDemoUpdate(const DxInput & input)
{
	if (input.IsKeyTrigger(VIRTUAL_KEY_INDEX::KEY_LBUTTON))
	{
		auto windowSize = Dx12Ctrl::Instance().GetWindowSize();
		auto size = mCube->GetImageSize() * SCALE * 0.5f;
		auto mousePos = input.GetMousePos();


		if (mousePos.x < windowSize.x &&
			mousePos.x > windowSize.x - size.x * 2.0f &&
			mousePos.y > 0 &&
			mousePos.y < size.y * 2.0f)
		{
			mScene = std::make_shared<FreeCutScene>(0);
			auto windowSizeX = Dx12Ctrl::Instance().GetWindowSize().x;
			InitIcon(mModeIcon, "Game/Resource/FreeMode.png", windowSizeX - mCone->GetImageSize().x * SCALE);
			mScene->Initialize();
			mUpdateState = &StarBreakApp::FreeCutDemoUpdate;
		}
		else
		{
			DirectX::XMFLOAT2 pos = { size.x, size.y };

			for (unsigned int i = 0; i < 4; ++i)
			{
				if (mousePos.x < pos.x + size.x &&
					mousePos.x > pos.x - size.x &&
					mousePos.y < pos.y + size.y &&
					mousePos.y > pos.y - size.y)
				{
					mScene = std::make_shared<GameScene>(static_cast<GameScene::PrimitiveType>(i));
					mScene->Initialize();
					break;
				}
				pos.x += size.x * 2.0f;
			}
		}
	}
}

void StarBreakApp::FreeCutDemoUpdate(const DxInput & input)
{
	if (input.IsKeyTrigger(VIRTUAL_KEY_INDEX::KEY_LBUTTON))
	{
		auto windowSize = Dx12Ctrl::Instance().GetWindowSize();
		auto size = mCube->GetImageSize() * SCALE * 0.5f;
		auto mousePos = input.GetMousePos();

		if (mousePos.x < windowSize.x &&
			mousePos.x > windowSize.x - size.x * 2.0f&&
			mousePos.y > 0 &&
			mousePos.y < size.y * 2.0f)
		{
			mScene = std::make_shared<GameScene>(static_cast<GameScene::PrimitiveType>(0));
			mScene->Initialize();
			auto windowSizeX = Dx12Ctrl::Instance().GetWindowSize().x;
			InitIcon(mModeIcon, "Game/Resource/GameMode.png", windowSizeX - mCone->GetImageSize().x * SCALE);
			mUpdateState = &StarBreakApp::GameDemoUpdate;
		}
		else
		{
			DirectX::XMFLOAT2 pos = { size.x, size.y };

			for (unsigned int i = 0; i < 4; ++i)
			{
				if (mousePos.x < pos.x + size.x &&
					mousePos.x > pos.x - size.x &&
					mousePos.y < pos.y + size.y &&
					mousePos.y > pos.y - size.y)
				{
					mScene = std::make_shared<FreeCutScene>(i);
					mScene->Initialize();
					break;
				}
				pos.x += size.x * 2.0f;
			}
		}
	}
}

void StarBreakApp::SetOffsetLeftUp(std::shared_ptr<ImageController>& ctrl)
{
	auto halfsize = ctrl->GetImageSize() * 0.5f * SCALE;
	ctrl->SetCenterOffset(-halfsize.x / 2.0f, halfsize.y / 2.0f, 0.0f);
}

void StarBreakApp::InitImages()
{
	const std::string relativePath = "Game/Resource/";
	float offset = 0.0f;
	InitIcon(mCube, relativePath + "cube.png", offset);
	offset += mCube->GetImageSize().x * SCALE;

	InitIcon(mSphere, relativePath + "sphere.png", offset);
	offset += mSphere->GetImageSize().x * SCALE;

	InitIcon(mCapsule, relativePath + "capsule.png", offset);
	offset += mCapsule->GetImageSize().x * SCALE;

	InitIcon(mCone, relativePath + "cone.png", offset);

	auto windowSizeX = Dx12Ctrl::Instance().GetWindowSize().x;
	InitIcon(mModeIcon, relativePath + "GameMode.png", windowSizeX - mCone->GetImageSize().x * 0.5f);
}

void StarBreakApp::InitIcon(std::shared_ptr<ImageController>& ctrl, const std::string & path, const float offset)
{
	auto windowSize = Dx12Ctrl::Instance().GetWindowSize();
	ctrl = LoadImage2D(path);
	SetOffsetLeftUp(ctrl);
	ctrl->SetPos(-windowSize.x / 2.0f + offset, windowSize.y / 2.0f, 0.0f);
	ctrl->SetScale(SCALE);
}

