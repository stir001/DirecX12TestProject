#pragma once
#include <vector>
#include <Dx12MSLib.h>

namespace ReMesh {
	struct Triangle
	{
		PrimitiveVertex verts[3];
		unsigned int vertNo[3];
	};

	using FaceTriangleGroup = std::map<DirectX::XMFLOAT4, std::list<Triangle>>;
	using FaceLineGroup = std::map<DirectX::XMFLOAT3, std::list<std::pair<unsigned int, unsigned int>>>;
}

class ReMeshConverter
{
public:
	ReMeshConverter();
	~ReMeshConverter();

	struct Mesh
	{
		std::vector<PrimitiveVertex> verts;
		std::vector<unsigned int> indiecs;
	};

	Mesh ReMesh(const std::vector<PrimitiveVertex>& verts, const std::vector<unsigned int>& indices);
private:
	ReMesh::FaceTriangleGroup GroupingFaceTriangle(const std::vector<PrimitiveVertex>& verts, const std::vector<unsigned int>& indices);
	ReMesh::FaceLineGroup GroupingLine(ReMesh::FaceTriangleGroup& group);
};

