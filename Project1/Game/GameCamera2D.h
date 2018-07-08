#pragma once
#include <DirectXMath.h>
#include <map>
#include <memory>

class IDrawableObject;

class GameCamera2D
{
public:
	GameCamera2D();
	~GameCamera2D();

	void DrawObjects();
	int SetObject(std::shared_ptr<IDrawableObject>& drawobject);
	void RemoveDrawObject(int objectNum);
private:
	int mObjectsNum;
	DirectX::XMFLOAT3 mCameraPos;
	std::map<int, std::weak_ptr<IDrawableObject>> mObjects;
};

