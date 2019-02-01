#include "MoveRestrictionMesh.h"
#include "RandomGenerator.h"
#include "TriangleDef.h"
#include <Dx12MSLib.h>


MoveRestrictionMesh::MoveRestrictionMesh(unsigned int div)
{
	float radius = 80.0f;
	float range = 20.0f;

	CreateCustumMesh(radius, div, range, mOutRangeVerts, mOutRangeIndices);
	std::shared_ptr<PrimitiveObject> primitive = std::make_shared<PrimitiveObject>("OutRangeMesh", mOutRangeVerts, mOutRangeIndices);
	mOutRangeMesh = PrimitiveCreator::Instance().CreateCustumPrimitve(primitive);

	radius /= 2.0f;
	CreateCustumMesh(radius, div, range, mInRangeVerts, mInRangeIndices);
	primitive = std::make_shared<PrimitiveObject>("InRangeMesh", mInRangeVerts, mInRangeIndices);
	mInRangeMesh = PrimitiveCreator::Instance().CreateCustumPrimitve(primitive);
	mInRangeMesh->SetColor({ 0.5f, 1.0f, 0.0f, 1.0f });
	mInRangeMesh->SetPosition({ 0.0f, 0.001f, 0.0f });
}


MoveRestrictionMesh::~MoveRestrictionMesh()
{
}

void MoveRestrictionMesh::Draw()
{
	mOutRangeMesh->Draw();
	mInRangeMesh->Draw();
}

unsigned int MoveRestrictionMesh::GetOutRangePolygonNum() const
{
	return static_cast<unsigned int>(mOutRangeIndices.size()) / 3U;
}

unsigned int MoveRestrictionMesh::GetInRangePolygonNum() const
{
	return static_cast<unsigned int>(mInRangeIndices.size()) / 3U;
}

PRIMITIVE::Triangle MoveRestrictionMesh::GetOutRangePolygon(unsigned int polygonID) const
{
	PRIMITIVE::Triangle rtn;
	unsigned int pivoitVertIndex = polygonID * 3;
	rtn.pos1 = ConvertXMFloat4ToXMFloat3(mOutRangeVerts[mOutRangeIndices[pivoitVertIndex]].pos);
	rtn.pos2 = ConvertXMFloat4ToXMFloat3(mOutRangeVerts[mOutRangeIndices[pivoitVertIndex + 1]].pos);
	rtn.pos3 = ConvertXMFloat4ToXMFloat3(mOutRangeVerts[mOutRangeIndices[pivoitVertIndex + 2]].pos);
	return rtn;
}

PRIMITIVE::Triangle MoveRestrictionMesh::GetInRangePolygon(unsigned int polygonID) const
{
	PRIMITIVE::Triangle rtn;
	unsigned int pivoitVertIndex = polygonID * 3;
	rtn.pos1 = ConvertXMFloat4ToXMFloat3(mInRangeVerts[mInRangeIndices[pivoitVertIndex]].pos);
	rtn.pos2 = ConvertXMFloat4ToXMFloat3(mInRangeVerts[mInRangeIndices[pivoitVertIndex + 1]].pos);
	rtn.pos3 = ConvertXMFloat4ToXMFloat3(mInRangeVerts[mInRangeIndices[pivoitVertIndex + 2]].pos);
	return rtn;
}

void MoveRestrictionMesh::CreateCustumMesh(float radius, unsigned int div, float range, std::vector<PrimitiveVertex>& verts, std::vector<unsigned int>& indices)
{
	RandomGenerator generator;
	PrimitiveVertex vert;
	vert.normal = { 0.0f, 1.0f, 0.0f, 1.0f };
	float unitRad = (3.14159265f * 2) / div;

	verts.reserve(div + 2);
	verts.push_back(vert);//äÓèÄÇÃíÜêSì_
	for (unsigned int i = 0; i < div; ++i)
	{
		float vertRadius = generator.GetValue(radius - range, radius + range);
		vert.pos.x = sinf(unitRad * i) * vertRadius;
		vert.pos.z = cosf(unitRad * i) * vertRadius;
		verts.push_back(vert);
	}
	verts.push_back(verts[1]);

	indices.resize(div * 3);
	for (unsigned int i = 0; i < div; ++i)
	{
		unsigned int pivotIdxNum = i * 3;
		indices[pivotIdxNum] = 0;
		indices[pivotIdxNum + 1] = (i + 1);
		indices[pivotIdxNum + 2] = (i + 2);

	}
}
