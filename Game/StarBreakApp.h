#pragma once
#include <Windows.h>
#include <memory>

class Scene;
class DxInput;

class StarBreakApp
{
public:
	StarBreakApp();
	~StarBreakApp();
	void Initialize(HINSTANCE hInst);
	void Terminate();
	void Run();
private:
	void DemoUpdate(const DxInput& input);

	std::shared_ptr<Scene> mScene;
};

