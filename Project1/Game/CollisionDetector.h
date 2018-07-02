#pragma once
#include <memory>
#include <DirectXMath.h>

class Enemy;
class PlayerSH;
class Rect;

class CollisionDetector
{
public:
	CollisionDetector();
	~CollisionDetector();

	void CheckCollision(std::shared_ptr<PlayerSH>& c1, std::shared_ptr<Enemy>& c2);
	bool RectCollision(const Rect& r1,const DirectX::XMFLOAT3& r1offset,const bool isR1Turn, const Rect& r2,const DirectX::XMFLOAT3& r2offset,const bool isR2Turn);
private:
};

