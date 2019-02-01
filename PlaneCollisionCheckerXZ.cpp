#include "PlaneCollisionCheckerXZ.h"
#include "MoveRestrictionMesh.h"
#include "TriangleDef.h"
#include <Dx12MSLib.h>


PlaneCollisionCheckerXZ::PlaneCollisionCheckerXZ()
{
}


PlaneCollisionCheckerXZ::~PlaneCollisionCheckerXZ()
{
}

bool PlaneCollisionCheckerXZ::IsInTriangle(const DirectX::XMFLOAT3& point, const PRIMITIVE::Triangle & triangle) const
{
	auto p1Sine = Sign(CrossXZ(point - triangle.pos1, triangle.pos2 - triangle.pos1));
	auto p2Sine = Sign(CrossXZ(point - triangle.pos2, triangle.pos3 - triangle.pos2));
	auto p3Sine = Sign(CrossXZ(point - triangle.pos3, triangle.pos1 - triangle.pos3));

	return p1Sine == p2Sine && p2Sine == p3Sine;
}

float PlaneCollisionCheckerXZ::CrossXZ(const DirectX::XMFLOAT3 & val1, const DirectX::XMFLOAT3 & val2) const
{
	return val1.x * val2.z - val1.z * val2.x;
}

int PlaneCollisionCheckerXZ::Sign(float value) const
{
	int rtn = 0;
	if(value < 0)
	{
		rtn = -1;
	}
	else if (value > 0)
	{
		rtn = 1;
	}

	return rtn;
}
