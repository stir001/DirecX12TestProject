#include "DxInput.h"

//#include <Xinput.h>
#include <Windows.h>

#pragma comment(lib,"xinput.lib")

DxInput::DxInput()
{
	ZeroMemory(keyState, sizeof(keyState));
	GetKeyboardState(keyState);
	GetCursorPos(&mousePos.pos);
}


DxInput::~DxInput()
{
}

bool DxInput::UpdateKeyState()
{
	return GetKeyboardState(keyState) != 0;
}

const unsigned char* DxInput::GetKeyState()
{
	return keyState;
}

const MousePos DxInput::GetMousePos()
{
	GetCursorPos(&mousePos.pos);
	return mousePos;
}

bool DxInput::CheckKeyDown(VirturalKeyIndex index)
{
	return keyState[index] & ksc_DOWN;
}