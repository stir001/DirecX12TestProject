#include "DxInput.h"

//#include <Xinput.h>
#include <Windows.h>

#pragma comment(lib,"xinput.lib")

DxInput::DxInput()
{
	ZeroMemory(mPreKeyState, sizeof(mPreKeyState));
	GetKeyboardState(mKeyState);
	GetCursorPos(&mMousePos.pos);
}

DxInput::~DxInput()
{
}

bool DxInput::UpdateKeyState()
{
	memcpy(mPreKeyState, mKeyState, sizeof(mKeyState));
	return GetKeyboardState(mKeyState) != 0;
}

const unsigned char* DxInput::GetKeyState()
{
	return mKeyState;
}

const MousePos DxInput::GetMousePos()
{
	GetCursorPos(&mMousePos.pos);
	return mMousePos;
}

bool DxInput::IsKeyDown(eVIRTUAL_KEY_INDEX index)const
{
	return (mKeyState[index] & eKEY_STATE_CHECK_DOWN) != 0;
}

bool DxInput::IsKeyTrigger(eVIRTUAL_KEY_INDEX index)const
{
	return ((mKeyState[index] & eKEY_STATE_CHECK_TOGGLE) ^ (mPreKeyState[index] & eKEY_STATE_CHECK_TOGGLE)) != 0;
}

bool DxInput::IsKeyToggle(eVIRTUAL_KEY_INDEX index) const
{
	return (mKeyState[index] & eKEY_STATE_CHECK_TOGGLE) != 0;
}