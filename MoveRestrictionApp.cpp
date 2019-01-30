#include "MoveRestrictionApp.h"
#include <Dx12MSLib.h>


MoveRestrictionApp::MoveRestrictionApp()
{
}


MoveRestrictionApp::~MoveRestrictionApp()
{
}

void MoveRestrictionApp::Run()
{
	auto& camera = Dx12Ctrl::Instance().GetCameraHolder()->GetCamera(0);
	DxInput input;
	{
		auto cone = PrimitiveCreator::Instance().CreateCone();

		while (ProcessMessage()) {
			input.UpdateKeyState();

			cone->Draw();

			camera->DefaultMove(input);

		}


	}
	camera = nullptr;

	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
}

void MoveRestrictionApp::Initialize(HINSTANCE hInst)
{
#ifndef _DEBUG
	ShaderCompiler::Instance().SetShaderDirPath("Shader/");
#endif
	Dx12CtrlInit(hInst, "1601295_^“ç§ˆê˜Y");
}
