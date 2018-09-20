#pragma once
/**
*	@addtogroup PMD_Data
*	@file PMDDataStructure
*	@brief PMD�̏ڍׂȏ���ێ�����\���̂̒�`
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/20
*/
#include <vector>
#include <DirectXMath.h>

/**
*	@ingroup PMD_Data
*	@struct PMDHeader
*	@brief PMD�t�@�C���̃w�b�_�����̍\����
*/
struct PMDHeader
{
	/**
	*	PMD�Ƃ��������������Ă���w�b�_����
	*/
	char fileType[3];

	/**
	*	PMD�̃o�[�W�������
	*/
	float version;

	/**
	*	PMD���f���̖��O
	*/
	char name[20];

	/**
	*	PMD���f���ɂ��ẴR�����g
	*/
	char comment[256];

	/**
	*	���_��
	*/
	unsigned int vertexcount;
};

/**
*	@ingroup PMD_Data
*	@struct PMDVertex
*	@brief PMD�̒��_���
*/
struct PMDVertex
{
	/**
	*	���_���W
	*/
	DirectX::XMFLOAT3 pos;

	/**
	*	�@�����
	*/
	DirectX::XMFLOAT3 normal;

	/**
	*	uv���
	*/
	DirectX::XMFLOAT2 uv;

	/**
	*	�e�����󂯂�{�[���ԍ�
	*/
	unsigned short boneNum[2];

	/**
	*	�{�[���ԍ���0�Ԗڂ̃{�[������e�����󂯂�E�F�C�g
	*/
	unsigned char boneWeight;//�E�F�C�g(1�o�C�g) 
	unsigned char edgeFlag;//�֊s���t���O(1�o�C�g) 
};

/**
*	@ingroup PMD_Data
*	@struct �N���X��
*	@brief �N���X�̏ڍ�
*/
struct PMDMaterial {
	DirectX::XMFLOAT3 diffuse;
	float alpha;
	float specularity;
	DirectX::XMFLOAT3 specular;
	DirectX::XMFLOAT3 ambient;
	unsigned char toonIndex;
	unsigned char edgeFlag;
	unsigned int indexCount;
	char texturePath[20];
	int texid;
};

/**
*	@ingroup PMD_Data
*	@struct �N���X��
*	@brief �N���X�̏ڍ�
*/
struct PMDBoneData
{
	char boneName[20];//�{�[����
	unsigned short parentIndex;//�e�ԍ�
	unsigned short tailIndex;//�I�[�ԍ�
	unsigned char type;//���
	unsigned short ikParentIndex;//IK�e�ԍ�
	DirectX::XMFLOAT3 pos;//���W
};

/**
*	@ingroup PMD_Data
*	@struct �N���X��
*	@brief �N���X�̏ڍ�
*/
struct PMDIKData
{
	unsigned short ikBoneIndex;
	unsigned short ikTargetBoneIndex;
	unsigned char ikChainLength;//IK�`�F�[���̒���
	unsigned short iterations;//�ċN���Z�� IK�l1
	float controlWeight;//���Z��񓖂���̐����p�x IK�l2
	std::vector<unsigned short> ikChildBoneIndex;//IK�e�����̃{�[���ԍ�(�v�f����ikChainLength��)
};

/**
*	@ingroup PMD_Data
*	@struct �N���X��
*	@brief �N���X�̏ڍ�
*/
struct PMDIKList
{
	unsigned short ikCount;
	std::vector<PMDIKData> ikDatas;
};

/**
*	@ingroup PMD_Data
*	@struct �N���X��
*	@brief �N���X�̏ڍ�
*/
struct PMDSkinVertexData
{
	unsigned int index;
	DirectX::XMFLOAT3 pos;
};

/**
*	@ingroup PMD_Data
*	@struct �N���X��
*	@brief �N���X�̏ڍ�
*/
struct PMDSkinData
{
	char skinName[20];
	unsigned int skinvertexcount;
	char skintype;//0:base,1:�܂�,2:��,3:���b�v,4:���̑�
	std::vector<PMDSkinVertexData> vertexeis;
};

/**
*	@ingroup PMD_Data
*	@struct �N���X��
*	@brief �N���X�̏ڍ�
*/
struct PMDSkin
{
	unsigned short skincount;
	std::vector<PMDSkinData> datas;
};

/**
*	@ingroup PMD_Data
*	@struct �N���X��
*	@brief �N���X�̏ڍ�
*/
struct PMDSkinDisp
{
	unsigned char skindispcount;
	std::vector<unsigned short> skinIndices;
};

/**
*	@ingroup PMD_Data
*	@struct �N���X��
*	@brief �N���X�̏ڍ�
*/
struct PMDBoneFrameDispName
{
	char name[50];
};

/**
*	@ingroup PMD_Data
*	@struct �N���X��
*	@brief �N���X�̏ڍ�
*/
struct PMDBoneFrameDisp
{
	unsigned char boneDispNameCount;
	std::vector<PMDBoneFrameDispName> names;
};

/**
*	@ingroup PMD_Data
*	@struct �N���X��
*	@brief �N���X�̏ڍ�
*/
struct PMDBoneDispIndex
{
	unsigned short boneIndex;
	unsigned char boneDispFrameIndex;
};

/**
*	@ingroup PMD_Data
*	@struct
*	@brief
*/
struct PMDBoneDisp
{
	unsigned int boneocunt;//0�ȊO�̃{�[�����̍��v
	std::vector<PMDBoneDispIndex> boneDispIndices;
};

/**
*	@ingroup PMD_Data
*	@struct
*	@brief
*/
struct PMDToonPath
{
	char path[100][10];
};

/**
*	@ingroup PMD_Data
*	@struct
*	@brief
*/
struct PMDRigidBodyData
{
	char rigidBodyName[20];
	unsigned short rigidbodyRelBoneIndex;//�֘A�{�[���ԍ�
	unsigned char rigidbodyGroupIndex;
	unsigned short rigidbodyGroupTarget;
	unsigned char shapeType; // �`��F�^�C�v(0:���A1:���A2:�J�v�Z��) // 00 // ��
	float shapeW; // �`��F���a(��) // CD CC CC 3F // 1.6
	float shapeH; // �`��F���� // CD CC CC 3D // 0.1
	float shapeD; // �`��F���s // CD CC CC 3D // 0.1
	DirectX::XMFLOAT3 pos; // �ʒu�F�ʒu(x, y, z)
	DirectX::XMFLOAT3 rot; // �ʒu�F��](rad(x), rad(y), rad(z))���W�A��
	float weight; // ���f�[�^�F���� // 00 00 80 3F // 1.0
	float posDim; // ���f�[�^�F�ړ��� // 00 00 00 00
	float rotDim; // ���f�[�^�F��]�� // 00 00 00 00
	float recoil; // ���f�[�^�F������ // 00 00 00 00
	float friction; // ���f�[�^�F���C�� // 00 00 00 00
	unsigned char rigidbodyType; // ���f�[�^�F�^�C�v(0:Bone�Ǐ]�A1:�������Z�A2:�������Z(Bone�ʒu����)) // 00 // Bone�Ǐ]
};

/**
*	@ingroup PMD_Data
*	@struct
*	@brief
*/
struct PMDRigdBody
{
	unsigned int rigidbodycount;
	std::vector<PMDRigidBodyData> datas;
};

/**
*	@ingroup PMD_Data
*	@struct
*	@brief
*/
struct PMDJointData
{
	char jointName[20]; // ���f�[�^�F���� // �E��1
	unsigned int jointRigidbodyA; // ���f�[�^�F����A
	unsigned int jointRigidbodyB; // ���f�[�^�F����B
	DirectX::XMFLOAT3 jointPos; // ���f�[�^�F�ʒu(x, y, z) // ���f�[�^�F�ʒu�����ł��ݒ��
	DirectX::XMFLOAT3 jointRot; // ���f�[�^�F��](rad(x), rad(y), rad(z))
	DirectX::XMFLOAT3 constrainPos1; // �����F�ړ�1(x, y, z)
	DirectX::XMFLOAT3 constrainPos2; // �����F�ړ�2(x, y, z)
	DirectX::XMFLOAT3 constrainRot1; // �����F��]1(rad(x), rad(y), rad(z))
	DirectX::XMFLOAT3 constrainRot2; // �����F��]2(rad(x), rad(y), rad(z))
	DirectX::XMFLOAT3 springPos; // �΂ˁF�ړ�(x, y, z)
	DirectX::XMFLOAT3 springRot; // �΂ˁF��](rad(x), rad(y), rad(z))
};

/**
*	@ingroup PMD_Data
*	@struct
*	@brief
*/
struct PMDJoint
{
	unsigned int jointcount;
	std::vector<PMDJointData> datas;
};

/**
*	@ingroup PMD_Data
*	@struct
*	@brief
*/
struct BoneTree
{
	std::vector<std::vector<int>> node;
};