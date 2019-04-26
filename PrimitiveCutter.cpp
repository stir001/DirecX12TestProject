#include "PrimitiveCutter.h"
#include "Dx12MSLib.h"


PrimitiveCutter::PrimitiveCutter()
{
}


PrimitiveCutter::~PrimitiveCutter()
{
}

PrimitiveCutter::CutVerts PrimitiveCutter::CutPrimitive(const CutData& data)
{
	CutVerts cut;

	unsigned int vertNum = static_cast<unsigned int>(data.primitive.verts.size());
	unsigned int indxNum = static_cast<unsigned int>(data.primitive.indices.size());

	cut.minus.verts.reserve(vertNum);
	cut.minus.indices.reserve(indxNum);
	cut.plus.verts.reserve(vertNum);
	cut.plus.indices.reserve(indxNum);

	CutByFace(cut, data);

	return cut;
}

int PrimitiveCutter::Sign(const float val)
{
	int rtn = 0;
	if (val > 0)
	{
		rtn = 1;
	}
	else if (val < 0)
	{
		rtn = -1;
	}
	return rtn;
}

float PrimitiveCutter::DotNormal(const DirectX::XMFLOAT3& normal, const PrimitiveVertex& verts, const DirectX::XMFLOAT3& origin)
{
	return DotXMFloat3(normal, ConvertXMFloat4ToXMFloat3(verts.pos) - origin);
}

void PrimitiveCutter::CutByFace(CutVerts& cut, const CutData& data)
{
	unsigned int indxNum = static_cast<unsigned int>(data.primitive.indices.size());
	for (unsigned int i = 0; i < indxNum; i += 3)
	{
		Triangle tri(data, i);
		int val1 = Sign(DotNormal(data.face.normal, tri.vert1, data.face.origin));
		int val2 = Sign(DotNormal(data.face.normal, tri.vert2, data.face.origin));
		int val3 = Sign(DotNormal(data.face.normal, tri.vert3, data.face.origin));
		if (val1 == val2 && val2 == val3)
		{
			if (val1 > 0)
			{
				AddTriangle(cut.plus, tri, mOriginPlusVertNo);
			}
			else if (val1 < 0)
			{
				AddTriangle(cut.minus, tri, mOriginMinusVertNo);
			}
		}
		else
		{
			CutTriangle(cut,);
		}
	}
}

void PrimitiveCutter::CutTriangle(CutVerts& cutVerts, const CutData& face)
{
}

int PrimitiveCutter::IsExistVert(const std::vector<int>& vertexNumbers, int vertexNo)
{
	unsigned int vertNum = static_cast<unsigned int>(vertexNumbers.size());
	for(unsigned int i = 0; i < vertexNo; ++i)
	{
		if (vertexNo == vertexNumbers[i])
		{
			return i;
		}
	}
	return -1;
}

void PrimitiveCutter::AddTriangle(Primitive& addPrimitive, const Triangle& tri, const std::vector<int>& vertNumbers)
{
	AddVert(addPrimitive, vertNumbers, tri.vert1, tri.vert1No);
	AddVert(addPrimitive, vertNumbers, tri.vert2, tri.vert2No);
	AddVert(addPrimitive, vertNumbers, tri.vert3, tri.vert3No);
}

void PrimitiveCutter::AddVert(Primitive & addPrimitive, const std::vector<int>& vertNumbers, const PrimitiveVertex & vert, const int vertNo)
{
	int idx = -1;
	if ((idx = IsExistVert(vertNumbers, vertNo)) == -1)
	{
		addPrimitive.verts.emplace_back(PrimitiveVertex(vert));
		addPrimitive.indices.push_back(addPrimitive.verts.size() - 1);
	}
	else
	{
		addPrimitive.indices.push_back(idx);
	}
}

PrimitiveCutter::Triangle::Triangle(const CutData& data, int index)
	: vert1No(data.primitive.indices[index]), vert1(data.primitive.verts[vert1No])
	, vert2No(data.primitive.indices[index + 1]), vert2(data.primitive.verts[vert2No])
	, vert3No(data.primitive.indices[index + 2]), vert3(data.primitive.verts[vert3No])
{
}
