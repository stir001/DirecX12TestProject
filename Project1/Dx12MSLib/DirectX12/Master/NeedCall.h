#pragma once
#include<Windows.h>

unsigned int Roundup2Multiplier(unsigned int size);

void CallEndPerGameLoop();

void CallStartPerGameLoop();

bool ProcessMessage();

void Dx12CtrlInit(HINSTANCE hInst);