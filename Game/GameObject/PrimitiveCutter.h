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
	*	���_�ƃC���f�b�N�X���
	*/
	struct Primitive
	{
		std::vector<PrimitiveVertex> verts;
		std::vector<unsigned int> indices;
	};

	/**
	*	�ؒf������̒��_������
	*/
	struct CutVerts
	{
		Primitive minus;
		Primitive plus;
	};

	/**
	*	�ؒf�Ɏg�����ʏ��
	*/
	struct CutFace
	{
		DirectX::XMFLOAT3 origin;
		DirectX::XMFLOAT3 normal;
	};

	/**
	*	�ؒf���钸�_�Q�Ɛؒf���镽��
	*/
	struct CutData
	{
		CutFace face;
		Primitive primitive;
	};

	/**
	*	�ؒf�O�f�[�^�̔ԍ��ƒ��_���
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
	*	@brief	�l�̐����𔻒肷�� 0�̏ꍇ�̂�0��Ԃ�
	*	@param	val	�������肷��l
	*	@return	-1:��	1:��	0:0
	*/
	int Sign(const float val);

	/**
	*	@brief	origin����verts�ւ̃x�N�g����normal���������̑傫����Ԃ�
	*	@param	normal	�C�ӂ̒��o��������������
	*	@param	verts	���_
	*	@param	origin	�C�ӂ̋N�_
	*	@return	�����̑傫��
	*/
	float DotNormal(const DirectX::XMFLOAT3& normal, const PrimitiveVertex& verts, const DirectX::XMFLOAT3& origin);

	/**
	*	@brief	primitive�𕽖ʂŐؒf����
	*	@param	cut		�ؒf��̒��_�ۑ��p
	*	@param	data	�ؒf�p�̏��
	*/
	void CutByFace(CutVerts& cut, const CutData& data);
	
	/**
	*	@brief	�O�p�`��ؒf����
	*	@param	cutVerts	�ؒf������̒��_���O���[�s���O���ۑ����邽�߂�Primitive�Q
	*	@param	data	�ؒf���邽�߂̕��ʏ��Ɛؒf����Primitive���
	*	@param	index	�g�p����data����Primitive�̎O�p�`���`����ŏ���Index
	*	@param	offset	�O�p�`�̂���������ʂ̃O���[�v�ɂȂ�Index�̃I�t�Z�b�g
	*	@param	isSingleVertPlus	������̃O���[�v�ɂȂ钸�_��+�����ǂ���
	*/
	void CutTriangle(CutVerts& cutVerts, const CutData& data, int index, int offset, bool isSingleVertPlus);

	/**
	*	@brief	�w��̒��_�����łɐݒ肳��Ă��邩�ǂ����𔻒肷��
	*	@param[in]	vertexNumbers	���_�ԍ���o�^��������
	*	@param[in]	vertexNo		���肷�钸�_�ԍ�
	*	@return		���݂���ꍇ:vertexNumbers�̓Y����	���݂��Ȃ��ꍇ:-1
	*/
	int IsExistVert(const std::vector<int>& vertexNumbers, int vertexNo);

	/**
	*	@brief	�O�p�`��ǉ�����
	*	@param addPrimitive	�O�p�`��ǉ�����v���~�e�B�u
	*	@param tri	�ǉ�����Triangle
	*	@param vertNumbers	Primitive�ɒǉ�����Ă���O�p�`�̌��̃C���f�b�N�X
	*/
	void AddTriangle(Primitive& addPrimitive, const Triangle& tri, std::vector<int>& vertNumbers);

	/**
	*	@brief	�O�p�`��ǉ�����
	*	@param	addPrimitive �O�p�`��ǉ�����v���~�e�B�u
	*	@param	vertNumbers	Primitive�ɒǉ������Ă���O�p�`�̌��̃C���f�b�N�X
	*	@param	v1No	1�Ԓ��_�̌��̃C���f�b�N�X
	*	@param	v1		1�Ԓ��_���
	*	@param	v2No	2�Ԓ��_�̌��̃C���f�b�N�X
	*	@param	v2		2�Ԓ��_���
	*	@param	v3No	3�Ԓ��_�̌��̃C���f�b�N�X
	*	@param	v3		3�Ԓ��_���
	*/
	void AddTriangle(Primitive& addPrimitive, std::vector<int>& vertNumbers,
		int v1No, const PrimitiveVertex& v1, int v2No, const PrimitiveVertex& v2,
		int v3No, const PrimitiveVertex& v3);

	/**
	*	@brief	���_���v���~�e�B�u�ɒǉ����邻�̍�Index�������ɒǉ�����
	*	@param addPrimitive	���_��ǉ������
	*	@param vertNumbers	�ǉ�����Ă��钸�_�̃I���W�i���̒��_�ԍ�
	*	@param vert		�ǉ����钸�_���
	*	@param vertNo	�ǉ����钸�_�̃I���W�i���̒��_�ԍ�
	*/
	void AddVert(Primitive& addPrimitive, std::vector<int>& vertNumbers
		, const PrimitiveVertex& vert, const int vertNo);

	/**
	*	@brief	�ؒf���ɍ쐬����V�K���_���쐬����
	*	@param	single	�ؒf���̃O���[�s���O�ň�ɂȂ������_
	*	@param	v1		�ؒf���̃O���[�s���O�œ�ɂȂ������_�Q�̈�Ԗڂ̒��_
	*	@param	v2		�ؒf���̃O���[�s���O�œ�ɂȂ������_�Q�̓�Ԗڂ̒��_
	*	@param	face	�ؒf����Ƃ��Ɏg�p��������
	*	@return	�V�K���_1,�V�K���_2
	*/
	std::tuple<PrimitiveVertex, PrimitiveVertex> GetNewVerts(const PrimitiveVertex& single,
		const PrimitiveVertex& v1, const PrimitiveVertex& v2, const CutFace& face);

	/**
	*	@brief	�O���[�s���O�ň�ɂȂ钸�_�̃I�t�Z�b�g��Ԃ�
	*	@param	v1Val		1�Ԓ��_�̃O���[�s���O�̒l
	*	@param	v2Val		2�Ԓ��_�̃O���[�s���O�̒l
	*	@param	v3Val		3�Ԓ��_�̃O���[�s���O�̒l
	*	@return	�I�t�Z�b�g	0:v1	1:v2	2:v3
	*/
	int GetSingleVertOffset(int v1Val, int v2Val, int v3Val);

	/**
	*	@brief	�ؒf�ʂ��ӂ���
	*	@param	cut		�ؒf�������_��ۑ�����
	*	@param	data	�ؒf���邽�߂̏��
	*/
	void CoverCutFace(CutVerts& cut, const CutData& data);

	/**
	*	@brief	�O���[�s���O���Ƃɖʂ𒣂�
	*/
	void CreateGroupFace(Primitive& primitive, std::vector<PrimitiveVertex>& madeVertex, const DirectX::XMFLOAT3& normal);

	/**
	*	@brief ���_���O���[�s���O����
	*/
	std::vector<PrimitiveVertex> GetGroupVerts(std::vector<PrimitiveVertex>& verts
		, unsigned int pivoitIndex, std::vector<int>& vertexGroupID, const int groupID);

	/**
	*	@brief	�ʂ��\�����邽�߂̃C���f�b�N�X���쐬����
	*			���̍ېV�K���_����ǉ�����
	*/
	std::vector<unsigned int> CreateFaceIndex(std::vector<PrimitiveVertex>& verts, const DirectX::XMFLOAT3& normal, const unsigned int indexOffset);

	/**
	*	@brief	�Ō�̒��_�Z�b�g�̒��ɍŏ��̒��_�Z�b�g�ƍ��v������̂����Ȃ����ꍇ�ɍČ�������ۂ̊֐�
	*/
	int ReSearchGourpVerts(std::vector<int>& vertexGroupID, int groupID
		, std::vector<PrimitiveVertex>& groupVerts
		, const std::vector<PrimitiveVertex>& originalVerts);

	bool EqualEpsilon(const DirectX::XMFLOAT4& lval, const DirectX::XMFLOAT4& rval, const float epsilon = 0.0001f);

	bool EpualPairVertexPos(const std::pair<PrimitiveVertex,PrimitiveVertex>& pair1,
		const std::pair<PrimitiveVertex, PrimitiveVertex>& pair2);

	//void CreateGroupVert(std::vector<PrimitiveVertex>& groupVert, std::vector<vertexGroupID>);
};

