#pragma once
#include <Windows.h>
#include <memory>

class ThrowHitSimulator;

class HitRepulsionApp
{
public:
	HitRepulsionApp();
	~HitRepulsionApp();
	void Initialize(HINSTANCE hInst);
	void Run();
private:
	std::shared_ptr<ThrowHitSimulator> mSimulator;
};

