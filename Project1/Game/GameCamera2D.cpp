#include "GameCamera2D.h"



GameCamera2D::GameCamera2D():mObjectsNum(0)
{
}

GameCamera2D::~GameCamera2D()
{
}

void GameCamera2D::DrawObjects()
{
}

int GameCamera2D::SetObject(std::shared_ptr<IDrawableObject>& drawobject)
{
	mObjects[mObjectsNum++] = drawobject;
	return mObjectsNum - 1;
}

void GameCamera2D::RemoveDrawObject(int objectNum)
{
	auto itr = mObjects.find(objectNum);
	if (itr == mObjects.end())
	{
		mObjects.erase(itr);
	}
}
