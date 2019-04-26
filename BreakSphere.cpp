#include "BreakSphere.h"
#include <random>

BreakSphere::BreakSphere() : PrimitiveSphere(10.0f,10U)
{
	std::random_device device;
	std::mt19937 gen(device());
	std::uniform_real_distribution<float> rand(-1.0f, 1.0f);

	DirectX::XMFLOAT3 normal(rand(gen),rand(gen),rand(gen));
	normal = NormalizeXMFloat3(normal);

	DirectX::XMFLOAT3 origin(0.0f, 0.0f, 0.0f);

	auto& verts = PrimitiveSphere::GetVertices();
	auto& indices = PrimitiveSphere::GetIndices();
	
	unsigned int vertNum = static_cast<unsigned int>(verts.size());
	unsigned int indxNum = static_cast<unsigned int>(indices.size());

	std::vector<PrimitiveVertex> minusVerts;
	minusVerts.reserve(vertNum);
	std::vector<PrimitiveVertex> plusVerts;
	plusVerts.reserve(vertNum);

	for (unsigned int i = 0; i < indxNum; i += 3)
	{
		int val1 = Sign(DotNormal(normal, verts[indices[i]], origin));
		int val2 = Sign(DotNormal(normal, verts[indices[i + 1]], origin));
		int val3 = Sign(DotNormal(normal, verts[indices[i + 2]], origin));
		if (val1 == val2 && val2 == val3)
		{
			if (val1 > 0)
			{
				plusVerts.push_back(verts[indices[i]]);
				plusVerts.push_back(verts[indices[i + 1]]);
				plusVerts.push_back(verts[indices[i + 2]]);
			}
			else if (val1 < 0)
			{
				minusVerts.push_back(verts[indices[i]]);
				minusVerts.push_back(verts[indices[i + 1]]);
				minusVerts.push_back(verts[indices[i + 2]]);
			}
		}
		else
		{

		}
	}
}

BreakSphere::~BreakSphere()
{
}

float BreakSphere::DotNormal(const DirectX::XMFLOAT3 & normal, const PrimitiveVertex & verts, const DirectX::XMFLOAT3 & origin)
{
	return DotXMFloat3(normal, ConvertXMFloat4ToXMFloat3(verts.pos) - origin);
}

int BreakSphere::Sign(const float val)
{
	int rtn;

	if (val < 0)
	{
		rtn = -1;
	}
	else if (val > 0)
	{
		rtn = 1;
	}
	else
	{
		rtn = 0;
	}

	return rtn;
}

void BreakSphere::CutTriangle(CutVertData& vertData, int vertNo, int vertVal1, int vertVal2, int vertVal3, const CutFace& face)
{
	int singleVertNo = -1;
	int singleVertVal = 0;
	if (vertVal1 == vertVal2)
	{
		singleVertNo = vertNo + 2;
		singleVertVal = vertVal3;
	}
	else if (vertVal1 == vertVal3)
	{
		singleVertNo = vertNo + 1;
		singleVertVal = vertVal2;
	}
	else
	{
		singleVertNo = vertNo;
		singleVertVal = vertVal1;
	}

	if (singleVertVal == -1)
	{
		vertData.AddMinus(singleVertNo);
		int sub = singleVertNo - vertNo;
		const int vert1 = vertNo + ((sub + 1) % 3);
		const int vert2 = vertNo + ((sub + 2) % 3);
		vertData.AddPlus(vert1);
		vertData.AddPlus(vert2);

		const PrimitiveVertex& v1 = vertData.originalVerts[vert1];
		const PrimitiveVertex& v2 = vertData.originalVerts[vert2];
		const PrimitiveVertex& singleV = vertData.originalVerts[singleVertNo];

		auto vec1 = ConvertXMFloat4ToXMFloat3(v1.pos - singleV.pos);
		auto vec2 = ConvertXMFloat4ToXMFloat3(v2.pos - singleV.pos);

		auto vecOrigin = face.origin - singleV.pos;

		auto length1 = DotXMFloat3(NormalizeXMFloat3(vec1), vecOrigin) / GetLengthXMFloat3(vec1);

		vec1 *= length1;
	
		auto length2 = DotXMFloat3(NormalizeXMFloat3(vec2), vecOrigin) / GetLengthXMFloat3(vec2);

		vec2 *= length2;

		auto newVertPos1 = ConvertXMFloat4ToXMFloat3(v1.pos + vec1);
		auto newVertPos2 = ConvertXMFloat4ToXMFloat3(v2.pos + vec2);

		vertData.makedMinusVerts.emplace_back(PrimitiveVertex(newVertPos1, -face.normal, v1.uv));
		vertData.makedMinusVerts.emplace_back(PrimitiveVertex(newVertPos2, -face.normal, v1.uv));

		vertData.makedPlusVerts.emplace_back(PrimitiveVertex(newVertPos1, face.normal, v1.uv));
		vertData.makedPlusVerts.emplace_back(PrimitiveVertex(newVertPos2, face.normal, v1.uv));
	}
	else if (singleVertVal == 1)
	{
		vertData.AddPlus(singleVertNo);
		int sub = singleVertNo - vertNo;
		vertData.AddMinus(vertNo + ((sub + 1) % 3));
		vertData.AddMinus(vertNo + ((sub + 2) % 3));
	}

	
}

void BreakSphere::CutVertData::AddMinus(int vertNo)
{
	minusVerts.push_back(originalVerts[vertNo]);
}

void BreakSphere::CutVertData::AddPlus(int vertNo)
{
	plusVerts.push_back(originalVerts[vertNo]);
}

void BreakSphere::CutVertData::AddMisuTriangle(int vertNo1, int vertNo2, int vertNo3)
{
	AddMinus(vertNo1);
	AddMinus(vertNo2);
	AddMinus(vertNo3);


}

bool BreakSphere::CutVertData::IsInMinusVert(int vertNo) const
{
	bool rtn = false;
	for (auto idx : minusVertsIndices)
	{
		if (vertNo == idx)
		{
			rtn = true;
			break;
		}
	}
	return rtn;
}

bool BreakSphere::CutVertData::IsInPlusVert(int vertNo) const
{
	bool rtn = false;
	for (auto idx : plusVertsIndices)
	{
		if (vertNo == idx)
		{
			rtn = true;
			break;
		}
	}
	return rtn;
}
