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
	mMadeMinusVertex.clear();
	mMadePlusVertex.clear();
	mOriginPlusVertNo.clear();
	mOriginMinusVertNo.clear();

	CutByFace(cut, data);

	CoverCutFace(cut, data);

	return cut;
}

std::vector<PrimitiveVertex> PrimitiveCutter::GetCutLineVertex() const
{
	return mMadePlusVertex;
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
				mMadePlusVertex.push_back(tri.vert1);
				mMadePlusVertex.push_back(tri.vert2);
				mMadePlusVertex.push_back(tri.vert3);

				AddTriangle(cut.minus, tri, mOriginMinusVertNo);
				mMadeMinusVertex.push_back(tri.vert1);
				mMadeMinusVertex.push_back(tri.vert2);
				mMadeMinusVertex.push_back(tri.vert3);
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
					mMadePlusVertex.push_back(tri.GetVertex((offset + 1) % 3));
					mMadePlusVertex.push_back(tri.GetVertex((offset + 2) % 3));
				}
				else
				{
					AddTriangle(cut.minus, tri, mOriginMinusVertNo);
					mMadeMinusVertex.push_back(tri.GetVertex((offset + 1) % 3));
					mMadeMinusVertex.push_back(tri.GetVertex((offset + 2) % 3));
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

	madeSingleVertex.push_back(newVert2);
	madeSingleVertex.push_back(newVert1);

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
		return offset;
	}
	else if (v1Val == v3Val)
	{
		offset = 1;
		return offset;
	}
	else if(v2Val == v3Val)
	{
		offset = 0;
		return offset;
	}

	if (v1Val != 0)
	{
		offset = 0;
	}
	else if (v2Val != 0)
	{
		offset = 1;
	}

	return offset;
}

void PrimitiveCutter::CoverCutFace(CutVerts& cut, const CutData& data)
{
	CreateGroupFace(cut.plus, mMadePlusVertex, -data.face.normal);
	CreateGroupFace(cut.minus, mMadeMinusVertex, data.face.normal);
}

void PrimitiveCutter::CreateGroupFace(Primitive& primitive, std::vector<PrimitiveVertex>& madeVertex, const DirectX::XMFLOAT3 & normal)
{
	const unsigned int vertexNum = static_cast<unsigned int>(madeVertex.size());
	unsigned int group = 0;
	std::vector<int> groupID(vertexNum);
	std::fill(groupID.begin(), groupID.end(), -1);

	for(unsigned int i = 0; i < vertexNum;++i)
	{
		unsigned int pivot = UINT_MAX;
		for (unsigned int j= 0;j < vertexNum;++j)
		{
			if (groupID[j] == -1)
			{
				pivot = j;
				break;
			}
		}

		if (pivot == UINT_MAX)
		{
			break;
		}

		auto groupVerts = GetGroupVerts(madeVertex, pivot, groupID, group++);
		const unsigned int indexOffset = static_cast<unsigned int>(primitive.verts.size());
		auto indices = CreateFaceIndex(groupVerts, normal, indexOffset);

		auto primitiveVertsNum = primitive.verts.size();
		primitive.verts.resize(primitiveVertsNum + groupVerts.size());
		std::copy(groupVerts.begin(), groupVerts.end(), primitive.verts.begin() + primitiveVertsNum);

		auto primitiveIndexNum = primitive.indices.size();
		primitive.indices.resize(primitiveIndexNum + indices.size());
		std::copy(indices.begin(), indices.end(), primitive.indices.begin() + primitiveIndexNum);
	}
}

std::vector<PrimitiveVertex> PrimitiveCutter::GetGroupVerts(std::vector<PrimitiveVertex>& verts, unsigned int pivoitIndex, std::vector<int>& vertexGroupID, const int groupID)
{
	if((pivoitIndex % 2) != 0) return std::vector<PrimitiveVertex>();
	if (vertexGroupID[pivoitIndex] != -1)
	{
		return std::vector<PrimitiveVertex>();
	}
	struct VertexSet
	{
		PrimitiveVertex vert1;
		PrimitiveVertex vert2;
		VertexSet();
		VertexSet(const PrimitiveVertex& v1, const PrimitiveVertex& v2) 
			: vert1(v1), vert2(v2) {}
	};
	
	VertexSet set = VertexSet(verts[pivoitIndex],verts[pivoitIndex + 1]);
	const VertexSet firstSet = set;
	vertexGroupID[pivoitIndex] = groupID;
	vertexGroupID[pivoitIndex + 1] = groupID;

	unsigned int vertexNum = static_cast<unsigned int>(verts.size());
	std::vector<PrimitiveVertex> groupVert;
	groupVert.reserve(vertexNum);
	groupVert.push_back(set.vert1);
	groupVert.push_back(set.vert2);

	while (true)
	{
		for (int i = 0; i < static_cast<int>(vertexNum); i += 2)
		{
			const unsigned int v1Index = i;
			const unsigned int v2Index = i + 1;
			if (vertexGroupID[v1Index] != -1 || vertexGroupID[v2Index] != -1)
			{
				continue;
			}

			if (EpualPairVertexPos({ set.vert1,set.vert2 }, { verts[v1Index], verts[v2Index] }))
			{
				vertexGroupID[v1Index] = groupID;
				vertexGroupID[v2Index] = groupID;
				set.vert1 = verts[v1Index];
				set.vert2 = verts[v2Index];
				groupVert.push_back(set.vert1);
				groupVert.push_back(set.vert2);
				i = -2;
			}
		}

		unsigned int groupvertNum = static_cast<unsigned int>(groupVert.size());
		if (!EpualPairVertexPos({ groupVert[0], groupVert[1] }, { groupVert[groupvertNum - 1], groupVert[groupvertNum - 2] }))
		{
			int vertNum = ReSearchGourpVerts(vertexGroupID, groupID, groupVert, verts);
			if(vertNum == INT_MAX)
			{
				break;
			}
			set.vert1 = verts[vertNum];
			set.vert2 = verts[vertNum + 1];
		}
		else
		{
			break;
		}
	}

	return groupVert;
}

std::vector<unsigned int> PrimitiveCutter::CreateFaceIndex(std::vector<PrimitiveVertex>& verts, const DirectX::XMFLOAT3& normal, const unsigned int indexOffset)
{
	DirectX::XMFLOAT4 avePos = { 0.0f,0.0f,0.0f,0.0f };
	DirectX::XMFLOAT2 aveUV = { 0.0f,0.0f };
	unsigned int vertsNum = static_cast<unsigned int>(verts.size());
	std::vector<unsigned int> indices(vertsNum + vertsNum / 2U);

	const auto n = ConvertXMFloat3ToXMFloat4(normal);
	for (unsigned int i = 0; i < vertsNum; i += 2)
	{
		avePos += verts[i].pos;
		aveUV += verts[i].uv;
		verts[i].normal = n;

		avePos += verts[i + 1].pos;
		aveUV += verts[i + 1].uv;
		verts[i + 1].normal = n;

		unsigned int index = (i / 2U) * 3U;
		indices[index] = i + indexOffset;
		indices[index + 1] = i + 1 + indexOffset;
		indices[index + 2] = vertsNum + indexOffset;
	}

	avePos /= static_cast<float>(vertsNum);
	aveUV /= static_cast<float>(vertsNum);
	auto aveVert = PrimitiveVertex(ConvertXMFloat4ToXMFloat3(avePos), normal, aveUV);
	verts.push_back(aveVert);

	return indices;
}

int PrimitiveCutter::ReSearchGourpVerts(std::vector<int>& vertexGroupID, int groupID
	, std::vector<PrimitiveVertex>& groupVerts
	, const std::vector<PrimitiveVertex>& originalVerts)
{
	const int groupVertsNum = static_cast<int>(groupVerts.size());
	const int originVertNum = static_cast<int>(originalVerts.size());
	//0と最後に追加された頂点以外のgroupVertsの中で座標が一致する頂点を探す
	for (int i = groupVertsNum - 1; i > 0; i -= 2)
	{
		for (int j = 0; j < originVertNum; j += 2)
		{
			if (vertexGroupID[j] != -1)
			{
				continue;
			}

			if (EpualPairVertexPos({ groupVerts[i], groupVerts[i - 1] }, { originalVerts[j], originalVerts[j + 1] }))
			{
				return j;
			}
		}
	}

	return INT_MAX;
}

bool PrimitiveCutter::EqualEpsilon(const DirectX::XMFLOAT4 & lval, const DirectX::XMFLOAT4 & rval, const float epsilon)
{
	return std::fabsf(lval.x - rval.x) < epsilon
		&& std::fabsf(lval.y - rval.y) < epsilon
		&& std::fabsf(lval.z - rval.z) < epsilon
		&& std::fabsf(lval.w - rval.w) < epsilon;
}

bool PrimitiveCutter::EpualPairVertexPos(const std::pair<PrimitiveVertex, PrimitiveVertex>& pair1, const std::pair<PrimitiveVertex, PrimitiveVertex>& pair2)
{
	return EqualEpsilon(pair1.first.pos, pair2.first.pos)
		|| EqualEpsilon(pair1.first.pos, pair2.second.pos)
		|| EqualEpsilon(pair1.second.pos, pair2.first.pos)
		|| EqualEpsilon(pair1.second.pos, pair2.second.pos);
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

PrimitiveVertex PrimitiveCutter::Triangle::GetVertex(unsigned int vertexNo) const
{
	switch (vertexNo)
	{
	case 0:
		return vert1;
	case 1:
		return vert2;
	case 2:
		return vert3;
	default:
		break;
	}
	return PrimitiveVertex();
}
