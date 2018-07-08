#pragma once
#include <DirectXMath.h>
#include <map>
#include <memory>

class IDrawableObject;
class PlayerSH;
class BackGround;

class GameCamera2D
{
public:
	GameCamera2D(std::shared_ptr<PlayerSH>& player, std::shared_ptr<BackGround>& backGround);
	~GameCamera2D();

	void Update();
	void DrawObjects();
	int SetObject(std::shared_ptr<IDrawableObject> drawobject);
	void RemoveDrawObject(int objectNum);
private:
	const int WINDOW_SIZE_X = 768;
	const int STAGE_IMAGE_SIZE_X = 576;
	int mObjectsNum;
	int mStateImageNum;//イメージ何枚分までスクロールするか
	DirectX::XMFLOAT3 mCameraPos;
	std::map<int, std::weak_ptr<IDrawableObject>> mObjects;
	std::shared_ptr<PlayerSH> mPlayer;
	std::shared_ptr<BackGround> mBackGround;
};

