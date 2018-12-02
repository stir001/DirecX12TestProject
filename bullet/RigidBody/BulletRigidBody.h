#pragma once
/**
*	@file BulletRigidBody.h
*	@brief btRigidBody�̃��b�p�[�N���X��`
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/11/26
*/
#include <btBulletDynamicsCommon.h>
#include <memory>
#include <DirectXMath.h>

class BulletCollisionObject;
class PhysicsSystem;
class BulletCollisionShape;

enum class BulletCollisionState 
{
	STATIC = 1,			//!�ړ����Ȃ��Œ�I�u�W�F�N�g�@�����v�Z�Ȃ��@�����蔻�肠��
	KINEMATIC = 2,		//!�C�ӈړ��\�I�u�W�F�N�g�@�����v�Z�Ȃ�	�����蔻�肠��
	NON_CONTACT = 4,	//!�C�ӈړ��s�I�u�W�F�N�g	�d�͂̂�?����@�����蔻��Ȃ�
	CHARACTER = 16,		//!�C�ӈړ��s�I�u�W�F�N�g	�����v�Z����	�����蔻�肠��
};

/**
*	@class BulletRigidBody
*	@brief btRigidBody�̃��b�p�[�N���X
*/
class BulletRigidBody : public std::enable_shared_from_this<BulletRigidBody>
{
public:
	friend PhysicsSystem;

	/**
	*	�������Ȃ��R���X�g���N�^
	*/
	BulletRigidBody();

	/**
	*	@param[in]	collisionShape	rigidBody�ɐݒ肷��`��
	*	@param[in]	pos		�����ʒu
	*/
	BulletRigidBody(std::shared_ptr<BulletCollisionShape> collisionShape
		, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	virtual ~BulletRigidBody();

	/**
	*	btRigidBody���擾����
	*/
	virtual std::shared_ptr<btRigidBody> GetRigidBody();

	/**
	*	@brief	���ʂ�ݒ肷��
	*	@param[in]	mass	�ݒ肷�鎿��
	*/
	virtual void SetMass(float mass);

	/**
	*	@brief �ʒu��ݒ肷��	
				�u�Ԉړ��Ȃ̂œ����蔻�莞�ɕs����N����\������
	*	@param[in]	x	x���W
	*	@param[in]	y	y���W
	*	@param[in]	z	z���W
	*/
	virtual void Translate(float x, float y, float z);

	/**
	*	@brief �ʒu��ݒ肷��
				�u�Ԉړ��Ȃ̂œ����蔻�莞�ɕs����N����\������
	*	@param[in]	pos	�ݒ肷����W
	*/
	virtual void Translate(const DirectX::XMFLOAT3& pos);

	/**
	*	@brief	���[���h�ϊ��s����擾����
	*	@return	���[���h�ϊ��s��
	*/
	DirectX::XMFLOAT4X4 GetWorldTransform() const;

	/**
	*	@brief	���[���h�ϊ��s���ݒ肷��
	*	@param[in]	matrix	���[���h�ϊ��s��
	*/
	void SetWorldTransform(const DirectX::XMFLOAT4X4& matrix);

	/**
	*	@brief	�^�O���擾����
	*	@return	�ݒ肳�ꂽ�^�O
	*/
	int GetTag() const;


	/**
	*	@brief	�C�ӂ̃^�O��ݒ肷��
				�R���W�����̔��莞�ɃI�u�W�F�N�g�𔻕ʂ��邽�߂̃^�O
	*	@param[in]	tag		�ݒ肷��^�O
	*/
	void SetTag(int tag);

	/**
	*	�R���W�����̏�Ԃ�ύX����
	*/
	void SetCollisionState(BulletCollisionState state);
protected:
	/**
	*	btRigidBody���쐬����
	*/
	virtual void CreateRigidBody();

	/**
	*	rigidBody
	*/
	std::shared_ptr<btRigidBody> mRigidBody;

	/**
	*	collision�̌`��
	*/
	std::shared_ptr<BulletCollisionShape> mCollisionShape;

	/**
	*	���[�V�����X�e�[�g	���̂Ƃ̓����ɕK�v
	*/
	std::shared_ptr<btMotionState> mMotionState;

	/**
	*	�^�O
	*/
	int mTag;

	/**
	*	����
	*/
	btScalar mMass;
};
