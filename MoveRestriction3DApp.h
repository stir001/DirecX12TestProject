#pragma once
#include <memory>
#include <Windows.h>
#include <vector>

class PlayerCharacter;
class PrimitiveController;
class PlaneCollisionCheckerXZ;
struct PrimitiveVertex;

namespace PRIMITIVE
{
	struct Triangle;
}

class MoveRestriction3DApp
{
public:
	MoveRestriction3DApp();
	~MoveRestriction3DApp();

	void Initialize(HINSTANCE hInst);
	void Run();
private:
	void CreateGround();
	PRIMITIVE::Triangle GetPolygon(unsigned int polygonID) const;
	void CalYPosition();

	std::shared_ptr<PlayerCharacter> mPlayer;
	std::shared_ptr<PrimitiveController> mGround;
	std::shared_ptr<PlaneCollisionCheckerXZ> mChecker;
	std::vector<PrimitiveVertex> mGroundVerts;
	std::vector<unsigned int> mGroundIndices;
};

