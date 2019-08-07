#include "ReMeshConverter.h"
#include <map>
#include <list>

ReMeshConverter::ReMeshConverter()
{
}


ReMeshConverter::~ReMeshConverter()
{
}

ReMeshConverter::Mesh ReMeshConverter::ReMesh(const std::vector<PrimitiveVertex>& verts, const std::vector<unsigned int>& indices)
{
	auto triangleGroup = GroupingFaceTriangle(verts, indices);

	auto lineGroup = GroupingLine(triangleGroup);

	return Mesh();
}

ReMesh::FaceTriangleGroup ReMeshConverter::GroupingFaceTriangle(const std::vector<PrimitiveVertex>& verts, const std::vector<unsigned int>& indices)
{
	ReMesh::Triangle t;

	ReMesh::FaceTriangleGroup group;

	unsigned int indicesNum = static_cast<unsigned int>(indices.size());

	for (unsigned int i = 0; i < indicesNum; i += 3)
	{
		unsigned int index = 0;
		t.vertNo[index] = indices[i];
		t.verts[index] = verts[t.vertNo[index]];
		++index;
		t.vertNo[index] = indices[i + 1];
		t.verts[index] = verts[t.vertNo[index]];
		++index;
		t.vertNo[index] = indices[i + 2];
		t.verts[index] = verts[t.vertNo[index]];

		auto normal = (t.verts[0].normal + t.verts[1].normal + t.verts[2].normal) / 3.0f;
		group[normal].push_back(t);
	}

	return group;
}

ReMesh::FaceLineGroup ReMeshConverter::GroupingLine(ReMesh::FaceTriangleGroup & group)
{
	ReMesh::FaceLineGroup faceLineVec;

	for (auto& face : group)
	{
		for (auto& t : face.second)
		{
			auto lineVec1 = Normalize(ConvertToXMFloat3(t.verts[1].pos - t.verts[0].pos));
			auto lineVec2 = Normalize(ConvertToXMFloat3(t.verts[2].pos - t.verts[1].pos));
			auto lineVec3 = Normalize(ConvertToXMFloat3(t.verts[0].pos - t.verts[2].pos));

			faceLineVec[lineVec1].emplace_back(t.vertNo[0], t.vertNo[1]);
			faceLineVec[lineVec2].emplace_back(t.vertNo[1], t.vertNo[2]);
			faceLineVec[lineVec3].emplace_back(t.vertNo[2], t.vertNo[0]);
		}
	}

	return faceLineVec;
}
