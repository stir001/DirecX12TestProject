#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>

class PrimitiveController;
struct PrimitiveVertex;
namespace PRIMITIVE {
	struct Triangle;
};

class MoveRestrictionMesh
{
public:
	MoveRestrictionMesh(unsigned int div);
	~MoveRestrictionMesh();

	void Draw();

	unsigned int GetOutRangePolygonNum() const;
	unsigned int GetInRangePolygonNum() const;

	PRIMITIVE::Triangle GetOutRangePolygon(unsigned int polygonID) const;
	PRIMITIVE::Triangle GetInRangePolygon(unsigned int polygonID) const;

private:
	void CreateCustumMesh(float radius, unsigned int div, float range
		, std::vector<PrimitiveVertex>& verts, std::vector<unsigned int>& indices);
	std::shared_ptr<PrimitiveController> mOutRangeMesh;//外側の移動制限メッシュ
	std::vector<PrimitiveVertex> mOutRangeVerts;
	std::vector<unsigned int> mOutRangeIndices;

	std::shared_ptr<PrimitiveController> mInRangeMesh;//内側の移動制限メッシュ
	std::vector<PrimitiveVertex> mInRangeVerts;
	std::vector<unsigned int> mInRangeIndices;
};

