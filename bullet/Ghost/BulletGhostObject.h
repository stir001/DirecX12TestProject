#pragma once
#include <memory>

class btGhostObject;
class BulletCollisionShape;

class BulletGhostObject
{
public:
	BulletGhostObject();
	virtual ~BulletGhostObject();

	std::shared_ptr<btGhostObject> GetGhostObject();
	void SetCollisionShape(std::shared_ptr<BulletCollisionShape> collisionShape);
private:
	std::shared_ptr<btGhostObject> mGhost;
	std::shared_ptr<BulletCollisionShape> mShape;
};

