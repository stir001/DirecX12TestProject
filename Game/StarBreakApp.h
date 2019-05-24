#pragma once
#include <Windows.h>
#include <memory>

class Scene;

class StarBreakApp
{
public:
	StarBreakApp();
	~StarBreakApp();
	void Initialize(HINSTANCE hInst);
	void Terminate();
	void Run();
private:
	std::shared_ptr<Scene> mScene;
};

