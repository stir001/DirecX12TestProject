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
	*	���ƃf�[�^�̔ԍ��ƒ��_���
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
	*	@brief	�w��̒��_�����łɐݒ肳��Ă��邩�ǂ����𔻒肷��
	*	@param[in]	vertexNumbers	���_�ԍ���o�^��������
	*	@param[in]	vertexNo		���肷�钸�_�ԍ�
	*	@return		���݂���ꍇ:vertexNumbers�̓Y����	���݂��Ȃ��ꍇ:-1
	*/
	int IsExistVert(const std::vector<int>& vertexNumbers, int vertexNo);

	void AddTriangle(Primitive& addPrimitive, const Triangle& tri, const std::vector<int>& vertNumbers);

	void AddVert(Primitive& addPrimitive, const std::vector<int>& vertNumbers
		, const PrimitiveVertex& vert, const int vertNo);
};

