#pragma once
#include <vector>
#include <DirectXMath.h>
#include "Dx12MSLib.h"

class PrimitiveCutter
{
public:
	PrimitiveCutter();
	~PrimitiveCutter();

	/**
	*	頂点とインデックス情報
	*/
	struct Primitive
	{
		std::vector<PrimitiveVertex> verts;
		std::vector<unsigned int> indices;
	};

	/**
	*	切断した後の頂点を入れる
	*/
	struct CutVerts
	{
		Primitive minus;
		Primitive plus;
	};

	/**
	*	切断に使う平面情報
	*/
	struct CutFace
	{
		DirectX::XMFLOAT3 origin;
		DirectX::XMFLOAT3 normal;
	};

	/**
	*	切断する頂点群と切断する平面
	*/
	struct CutData
	{
		CutFace face;
		Primitive primitive;
	};

	/**
	*	切断前データの番号と頂点情報
	*/
	struct Triangle
	{
		int vert1No;
		PrimitiveVertex vert1;
		int vert2No;
		PrimitiveVertex vert2;
		int vert3No;
		PrimitiveVertex vert3;
		Triangle();
		Triangle(int v1No, const PrimitiveVertex& v1,
			int v2No, const PrimitiveVertex& v2,
			int v3No, const PrimitiveVertex& v3);
		Triangle(const CutData& data, int index);
		PrimitiveVertex GetVertex(unsigned int vertexNo) const;
	};

	CutVerts CutPrimitive(const CutData& data);

	std::vector<PrimitiveVertex> GetCutLineVertex() const;
private:
	std::vector<PrimitiveVertex> mMadePlusVertex;
	std::vector<PrimitiveVertex> mMadeMinusVertex;
	std::vector<int> mOriginPlusVertNo;
	std::vector<int> mOriginMinusVertNo;
	
	/**
	*	@brief	値の正負を判定する 0の場合のみ0を返す
	*	@param	val	正負判定する値
	*	@return	-1:負	1:正	0:0
	*/
	int Sign(const float val);

	/**
	*	@brief	originからvertsへのベクトルのnormal方向成分の大きさを返す
	*	@param	normal	任意の抽出したい方向成分
	*	@param	verts	頂点
	*	@param	origin	任意の起点
	*	@return	成分の大きさ
	*/
	float DotNormal(const DirectX::XMFLOAT3& normal, const PrimitiveVertex& verts, const DirectX::XMFLOAT3& origin);

	/**
	*	@brief	primitiveを平面で切断する
	*	@param	cut		切断後の頂点保存用
	*	@param	data	切断用の情報
	*/
	void CutByFace(CutVerts& cut, const CutData& data);
	
	/**
	*	@brief	三角形を切断する
	*	@param	cutVerts	切断した後の頂点をグルーピングし保存するためのPrimitive群
	*	@param	data	切断するための平面情報と切断するPrimitive情報
	*	@param	index	使用するdata内のPrimitiveの三角形を定義する最初のIndex
	*	@param	offset	三角形のうち一つだけ別のグループになるIndexのオフセット
	*	@param	isSingleVertPlus	一つだけのグループになる頂点が+側かどうか
	*/
	void CutTriangle(CutVerts& cutVerts, const CutData& data, int index, int offset, bool isSingleVertPlus);

	/**
	*	@brief	指定の頂点がすでに設定されているかどうかを判定する
	*	@param[in]	vertexNumbers	頂点番号を登録したもの
	*	@param[in]	vertexNo		判定する頂点番号
	*	@return		存在する場合:vertexNumbersの添え字	存在しない場合:-1
	*/
	int IsExistVert(const std::vector<int>& vertexNumbers, int vertexNo);

	/**
	*	@brief	三角形を追加する
	*	@param addPrimitive	三角形を追加するプリミティブ
	*	@param tri	追加するTriangle
	*	@param vertNumbers	Primitiveに追加されている三角形の元のインデックス
	*/
	void AddTriangle(Primitive& addPrimitive, const Triangle& tri, std::vector<int>& vertNumbers);

	/**
	*	@brief	三角形を追加する
	*	@param	addPrimitive 三角形を追加するプリミティブ
	*	@param	vertNumbers	Primitiveに追加さえている三角形の元のインデックス
	*	@param	v1No	1番頂点の元のインデックス
	*	@param	v1		1番頂点情報
	*	@param	v2No	2番頂点の元のインデックス
	*	@param	v2		2番頂点情報
	*	@param	v3No	3番頂点の元のインデックス
	*	@param	v3		3番頂点情報
	*/
	void AddTriangle(Primitive& addPrimitive, std::vector<int>& vertNumbers,
		int v1No, const PrimitiveVertex& v1, int v2No, const PrimitiveVertex& v2,
		int v3No, const PrimitiveVertex& v3);

	/**
	*	@brief	頂点をプリミティブに追加するその際Indexも同時に追加する
	*	@param addPrimitive	頂点を追加する先
	*	@param vertNumbers	追加されている頂点のオリジナルの頂点番号
	*	@param vert		追加する頂点情報
	*	@param vertNo	追加する頂点のオリジナルの頂点番号
	*/
	void AddVert(Primitive& addPrimitive, std::vector<int>& vertNumbers
		, const PrimitiveVertex& vert, const int vertNo);

	/**
	*	@brief	切断時に作成する新規頂点を作成する
	*	@param	single	切断時のグルーピングで一つになった頂点
	*	@param	v1		切断時のグルーピングで二つになった頂点群の一番目の頂点
	*	@param	v2		切断時のグルーピングで二つになった頂点群の二番目の頂点
	*	@param	face	切断するときに使用した平面
	*	@return	新規頂点1,新規頂点2
	*/
	std::tuple<PrimitiveVertex, PrimitiveVertex> GetNewVerts(const PrimitiveVertex& single,
		const PrimitiveVertex& v1, const PrimitiveVertex& v2, const CutFace& face);

	/**
	*	@brief	グルーピングで一つになる頂点のオフセットを返す
	*	@param	v1Val		1番頂点のグルーピングの値
	*	@param	v2Val		2番頂点のグルーピングの値
	*	@param	v3Val		3番頂点のグルーピングの値
	*	@return	オフセット	0:v1	1:v2	2:v3
	*/
	int GetSingleVertOffset(int v1Val, int v2Val, int v3Val);

	/**
	*	@brief	切断面をふさぐ
	*	@param	cut		切断した頂点を保存する
	*	@param	data	切断するための情報
	*/
	void CoverCutFace(CutVerts& cut, const CutData& data);

	/**
	*	@brief	グルーピングごとに面を張る
	*/
	void CreateGroupFace(Primitive& primitive, std::vector<PrimitiveVertex>& madeVertex, const DirectX::XMFLOAT3& normal);

	/**
	*	@brief 頂点をグルーピングする
	*/
	std::vector<PrimitiveVertex> GetGroupVerts(std::vector<PrimitiveVertex>& verts
		, unsigned int pivoitIndex, std::vector<int>& vertexGroupID, const int groupID);

	/**
	*	@brief	面を構成するためのインデックスを作成する
	*			その際新規頂点を一つ追加する
	*/
	std::vector<unsigned int> CreateFaceIndex(std::vector<PrimitiveVertex>& verts, const DirectX::XMFLOAT3& normal, const unsigned int indexOffset);

	/**
	*	@brief	最後の頂点セットの中に最初の頂点セットと合致するものがあなった場合に再検索する際の関数
	*/
	int ReSearchGourpVerts(std::vector<int>& vertexGroupID, int groupID
		, std::vector<PrimitiveVertex>& groupVerts
		, const std::vector<PrimitiveVertex>& originalVerts);

	bool EqualEpsilon(const DirectX::XMFLOAT4& lval, const DirectX::XMFLOAT4& rval, const float epsilon = 0.0001f);

	bool EpualPairVertexPos(const std::pair<PrimitiveVertex,PrimitiveVertex>& pair1,
		const std::pair<PrimitiveVertex, PrimitiveVertex>& pair2);

	//void CreateGroupVert(std::vector<PrimitiveVertex>& groupVert, std::vector<vertexGroupID>);
};

