#pragma once
#include <vector>
#include <Windows.h>

class DxInput;
class CapsuleCollider;
class SphereCollider;
class RandomGenerator;
class CollisionChecker;

class CollisionTestApp
{
public:
	CollisionTestApp();
	~CollisionTestApp();
	void Initialize(HINSTANCE hInst);
	void Run();
private:
};

