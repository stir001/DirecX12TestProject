#pragma once
#include <Windows.h>
#include <memory>
#include <string>

class Scene;
class DxInput;
class ImageController;

class StarBreakApp
{
public:
	StarBreakApp();
	~StarBreakApp();
	void Initialize(HINSTANCE hInst);
	void Terminate();
	void Run();
private:
	void DemoDraw();
	void GameDemoUpdate(const DxInput& input);
	void FreeCutDemoUpdate(const DxInput& input);

	void SetOffsetLeftUp(std::shared_ptr<ImageController>& ctrl);
	void InitImages();
	void InitIcon(std::shared_ptr<ImageController>& ctrl, const std::string& path, const float offset);

	void (StarBreakApp::*mUpdateState)(const DxInput& input);
	std::shared_ptr<Scene> mScene;
	std::shared_ptr<ImageController> mImg;

	std::shared_ptr<ImageController> mCube;
	std::shared_ptr<ImageController> mSphere;
	std::shared_ptr<ImageController> mCapsule;
	std::shared_ptr<ImageController> mCone;

	std::shared_ptr<ImageController> mModeIcon;
};