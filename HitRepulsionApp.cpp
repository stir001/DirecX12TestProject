#include "HitRepulsionApp.h"
#include "ThrowHitSimulator.h"
#include <Dx12MSLib.h>


HitRepulsionApp::HitRepulsionApp()
{
}


HitRepulsionApp::~HitRepulsionApp()
{
}

void HitRepulsionApp::Initialize(HINSTANCE hInst)
{
#ifndef _DEBUG
	ShaderCompiler::Instance().SetShaderDirPath("Shader/");
#endif
	Dx12CtrlInit(hInst, "1601295_ê^ìÁèßàÍòY");
	mSimulator = std::make_shared<ThrowHitSimulator>();
}

void HitRepulsionApp::Run()
{
	auto& camera = Dx12Ctrl::Instance().GetCameraHolder()->GetCamera(0);
	DxInput input;
	{
		camera->SetPos({ 0.0f, 150.0f, -100.0f });
		while (ProcessMessage()) {
			input.UpdateKeyState();
			mSimulator->Simulation(input);
			mSimulator->Draw();
		}
	}
	camera = nullptr;

	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
}
