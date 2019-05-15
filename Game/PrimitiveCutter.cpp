#include "PrimitiveCutter.h"
#include "Dx12MSLib.h"
#include <functional>
#include <cmath>

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
			else
			{
				AddTriangle(cut.plus, tri, mOriginPlusVertNo);
				AddTriangle(cut.minus, tri, mOriginMinusVertNo);
			}
		}
		else
		{
			int offset = GetSingleVertOffset(val1, val2, val3);
			const unsigned int VALUE_NUM = 3U;
			int values[VALUE_NUM] = { val1, val2, val3 };
			bool isPlus = values[offset] > 0;

			if (values[offset] == 0)
			{
				if (values[(offset + 1) % 3] > 0)
				{
					AddTriangle(cut.plus, tri, mOriginPlusVertNo);
				}
				else
				{
					AddTriangle(cut.minus, tri, mOriginMinusVertNo);
				}
			}
			else if (values[(offset + 1) % 3] == 0)
			{
				if (isPlus)
				{
					AddTriangle(cut.plus, tri, mOriginPlusVertNo);
				}
				else
				{
					AddTriangle(cut.minus, tri, mOriginMinusVertNo);
				}
			}
			else
			{
				CutTriangle(cut, data, i, offset, isPlus);
			}
		}
	}
}

void PrimitiveCutter::CutTriangle(CutVerts& cutVerts, const CutData& data, int index, int offset, bool isSingleVertPlus)
{
	//if (!isSingleVertPlus) return;
	//オリジナルプリミティブの頂点番号
	int singleVertNo = data.primitive.indices[index + offset];
	auto& singleOriginVertNo = isSingleVertPlus ? mOriginPlusVertNo : mOriginMinusVertNo;
	auto& otherOriginVertNo = isSingleVertPlus ? mOriginMinusVertNo : mOriginPlusVertNo;
	auto& singleAddVert = isSingleVertPlus ? cutVerts.plus : cutVerts.minus;
	auto& otherAddVert = isSingleVertPlus ? cutVerts.minus : cutVerts.plus;
	auto& madeSingleVertex = isSingleVertPlus ? mMadePlusVertex : mMadeMinusVertex;
	auto& madeOtherVertex = isSingleVertPlus ? mMadeMinusVertex : mMadePlusVertex;

	std::function<int(void)> getAmountVertsNum = [&data, &madeSingleVertex, &madeOtherVertex]() {
		return static_cast<int>(data.primitive.verts.size() + madeSingleVertex.size() + madeOtherVertex.size());
	};

	const int otherVert1No = data.primitive.indices[index + ((offset + 1) % 3)];
	const int otherVert2No = data.primitive.indices[index + ((offset + 2) % 3)];

	const PrimitiveVertex& otherV1 = data.primitive.verts[otherVert1No];
	const PrimitiveVertex& otherV2 = data.primitive.verts[otherVert2No];
	const PrimitiveVertex& singleV = data.primitive.verts[singleVertNo];

	auto [newVert1, newVert2] = GetNewVerts(singleV, otherV1, otherV2, data.face);

	int amountVertNum = getAmountVertsNum();

	AddTriangle(singleAddVert, singleOriginVertNo, amountVertNum,
		newVert1, amountVertNum + 1, newVert2, singleVertNo, data.primitive.verts[singleVertNo]);

	madeSingleVertex.push_back(newVert1);
	madeSingleVertex.push_back(newVert2);

	amountVertNum = getAmountVertsNum();
	AddTriangle(otherAddVert, otherOriginVertNo,
		amountVertNum, newVert1, otherVert1No, otherV1, otherVert2No, otherV2);

	AddTriangle(otherAddVert, otherOriginVertNo,
		amountVertNum, newVert1, otherVert2No, otherV2, amountVertNum + 1, newVert2);

	madeOtherVertex.push_back(newVert1);
	madeOtherVertex.push_back(newVert2);
}

int PrimitiveCutter::IsExistVert(const std::vector<int>& vertexNumbers, int vertexNo)
{
	unsigned int vertNum = static_cast<unsigned int>(vertexNumbers.size());
	for(unsigned int i = 0; i < vertNum; ++i)
	{
		if (vertexNo == vertexNumbers[i])
		{
			return i;
		}
	}
	return -1;
}

void PrimitiveCutter::AddTriangle(Primitive& addPrimitive, const Triangle& tri, std::vector<int>& vertNumbers)
{
	AddVert(addPrimitive, vertNumbers, tri.vert1, tri.vert1No);
	AddVert(addPrimitive, vertNumbers, tri.vert2, tri.vert2No);
	AddVert(addPrimitive, vertNumbers, tri.vert3, tri.vert3No);
}

void PrimitiveCutter::AddTriangle(Primitive & addPrimitive, std::vector<int>& vertNumbers,
	int v1No, const PrimitiveVertex & v1, int v2No, const PrimitiveVertex & v2,
	int v3No, const PrimitiveVertex & v3)
{
	Triangle tri(v1No, v1, v2No, v2, v3No, v3);
	AddTriangle(addPrimitive, tri, vertNumbers);
}

void PrimitiveCutter::AddVert(Primitive & addPrimitive, std::vector<int>& vertNumbers, const PrimitiveVertex & vert, const int vertNo)
{
	int idx = -1;
	if ((idx = IsExistVert(vertNumbers, vertNo)) == -1)
	{//存在しないとき
		addPrimitive.indices.push_back(static_cast<unsigned int>(addPrimitive.verts.size()));
		addPrimitive.verts.emplace_back(PrimitiveVertex(vert));
		vertNumbers.push_back(vertNo);
	}
	else
	{//存在するとき
		addPrimitive.indices.push_back(idx);
	}
}

std::tuple<PrimitiveVertex, PrimitiveVertex> PrimitiveCutter::GetNewVerts(const PrimitiveVertex & single
	, const PrimitiveVertex & v1, const PrimitiveVertex & v2, const CutFace & face)
{
	//座標の変化量
	auto vec1 = ConvertXMFloat4ToXMFloat3(v1.pos - single.pos);
	auto vec2 = ConvertXMFloat4ToXMFloat3(v2.pos - single.pos);

	//uvの変化量
	auto uv1vec = v1.uv - single.uv;
	auto uv2vec = v2.uv - single.uv;

	//法線の変化量
	auto normal1Vec = v1.normal - single.normal;
	auto normal2Vec = v2.normal - single.normal;

	auto singleLen = std::fabsf(DotXMFloat3(face.normal, face.origin - single.pos));
	auto v1Len = std::fabsf(DotXMFloat3(face.normal, face.origin - v1.pos));
	auto v2Len = std::fabsf(DotXMFloat3(face.normal, face.origin - v2.pos));

	auto singleV1faceRate = singleLen / (singleLen + v1Len);
	auto singleV2faceRate = singleLen / (singleLen + v2Len);

	auto newVertPos1 = ConvertXMFloat4ToXMFloat3(single.pos + vec1 * singleV1faceRate);
	auto newVertPos2 = ConvertXMFloat4ToXMFloat3(single.pos + vec2 * singleV2faceRate);

	auto newVertUV1 = single.uv + uv1vec * singleV1faceRate;
	auto newVertUV2 = single.uv + uv2vec * singleV2faceRate;

	auto newNormal1 = ConvertXMFloat4ToXMFloat3(single.normal + normal1Vec * singleV1faceRate);
	auto newNormal2 = ConvertXMFloat4ToXMFloat3(single.normal + normal2Vec * singleV2faceRate);

	//新規頂点を作成
	auto newVert1 = PrimitiveVertex(newVertPos1, newNormal1, newVertUV1);
	auto newVert2 = PrimitiveVertex(newVertPos2, newNormal2, newVertUV2);
	return { newVert1, newVert2 };
}

int PrimitiveCutter::GetSingleVertOffset(int v1Val, int v2Val, int v3Val)
{
	int offset = -1;
	if (v1Val == v2Val)
	{
		offset = 2;
	}
	else if (v1Val == v3Val)
	{
		offset = 1;
	}
	else
	{
		offset = 0;
	}

	return offset;
}

void PrimitiveCutter::BlockCutFace(CutVerts & cut, const CutData & data)
{
}

void PrimitiveCutter::CreateGroupFace(Primitive& primitive, std::vector<PrimitiveVertex>& madeVertex, const DirectX::XMFLOAT3 & normal)
{
	const unsigned int indexOffset = static_cast<unsigned int>(primitive.verts.size() - 1);
	const unsigned int vertexNum = static_cast<unsigned int>(madeVertex.size());
	unsigned int group = 0;
	struct VertSet
	{
		PrimitiveVertex v1;
		PrimitiveVertex v2;
	};
	std::vector<PrimitiveVertex> groupVerts;
	std::vector<int> groupID(vertexNum);
	std::fill(groupID.begin(), groupID.end(), -1);
	groupVerts.resize(vertexNum);
	groupVerts.emplace_back(PrimitiveVertex(ConvertXMFloat4ToXMFloat3(madeVertex[0].pos), normal, madeVertex[0].uv));
	groupVerts.emplace_back(PrimitiveVertex(ConvertXMFloat4ToXMFloat3(madeVertex[1].pos), normal, madeVertex[1].uv));
	groupID[0] = group;
	groupID[1] = group;
	for (unsigned int i = 2U; i < vertexNum; i += 2)
	{
		
	}
}

std::vector<PrimitiveVertex> PrimitiveCutter::GetGroupVerts(std::vector<PrimitiveVertex>& verts, unsigned int pivoitIndex, std::vector<int>& vertexGroupID, const int groupID)
{
	if((pivoitIndex % 2) != 0) return std::vector<PrimitiveVertex>();
	struct VertexSet
	{
		PrimitiveVertex vert1;
		PrimitiveVertex vert2;
		VertexSet();
		VertexSet(const PrimitiveVertex& v1, const PrimitiveVertex& v2) 
			: vert1(v1), vert2(v2) {}
	};
	if (vertexGroupID[pivoitIndex] != -1) return std::vector<PrimitiveVertex>();

	VertexSet set;

	return std::vector<PrimitiveVertex>();
}

PrimitiveCutter::Triangle::Triangle():vert1No(-1),vert2No(-1),vert3No(-1)
{
}

PrimitiveCutter::Triangle::Triangle(int v1No, const PrimitiveVertex & v1
	, int v2No, const PrimitiveVertex & v2, int v3No, const PrimitiveVertex & v3)
	: vert1No(v1No),vert1(v1),vert2No(v2No),vert2(v2),vert3No(v3No),vert3(v3)
{
}

PrimitiveCutter::Triangle::Triangle(const CutData& data, int index)
	: vert1No(data.primitive.indices[index]), vert1(data.primitive.verts[vert1No])
	, vert2No(data.primitive.indices[index + 1]), vert2(data.primitive.verts[vert2No])
	, vert3No(data.primitive.indices[index + 2]), vert3(data.primitive.verts[vert3No])
{
}
