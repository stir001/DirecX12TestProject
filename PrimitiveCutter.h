#pragma once
#include <vector>
#include <DirectXMath.h>

struct PrimitiveVertex;

class PrimitiveCutter
{
public:
	PrimitiveCutter();
	~PrimitiveCutter();

	struct Primitive
	{
		std::vector<PrimitiveVertex> verts;
		std::vector<int> indices;
	};

	struct CutVerts
	{
		Primitive minus;
		Primitive plus;
	};

	struct CutFace
	{
		DirectX::XMFLOAT3 origin;
		DirectX::XMFLOAT3 normal;
	};

	struct CutData
	{
		CutFace face;
		Primitive primitive;
	};

	/**
	*	もとデータの番号と頂点情報
	*/
	struct Triangle
	{
		int vert1No;
		PrimitiveVertex vert1;
		int vert2No;
		PrimitiveVertex vert2;
		int vert3No;
		PrimitiveVertex vert3;
		Triangle(const CutData& data, int index);
	};

	CutVerts CutPrimitive(const CutData& data);

private:
	std::vector<PrimitiveVertex> mMakedVertex;
	std::vector<int> mOriginPlusVertNo;
	std::vector<int> mOriginMinusVertNo;
	
	int Sign(const float val);
	float DotNormal(const DirectX::XMFLOAT3& normal, const PrimitiveVertex& verts, const DirectX::XMFLOAT3& origin);
	void CutByFace(CutVerts& cut, const CutData& data);
	void CutTriangle(CutVerts& cutVerts, const CutData& face);

	/**
	*	@brief	指定の頂点がすでに設定されているかどうかを判定する
	*	@param[in]	vertexNumbers	頂点番号を登録したもの
	*	@param[in]	vertexNo		判定する頂点番号
	*	@return		存在する場合:vertexNumbersの添え字	存在しない場合:-1
	*/
	int IsExistVert(const std::vector<int>& vertexNumbers, int vertexNo);

	void AddTriangle(Primitive& addPrimitive, const Triangle& tri, const std::vector<int>& vertNumbers);

	void AddVert(Primitive& addPrimitive, const std::vector<int>& vertNumbers
		, const PrimitiveVertex& vert, const int vertNo);
};

