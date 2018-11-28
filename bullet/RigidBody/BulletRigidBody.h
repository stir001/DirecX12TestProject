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

class PhysicsSystem;

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
	BulletRigidBody(std::shared_ptr<btCollisionShape> collisionShape
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
protected:
	/**
	*	btRigidBody���쐬����
	*/
	virtual void CreateRigidBody(bool isContlrable = false);

	/**
	*	rigidBody
	*/
	std::shared_ptr<btRigidBody> mRigidBody;

	/**
	*	collision�̌`��
	*/
	std::shared_ptr<btCollisionShape> mCollisionShape;

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

	/**
	*	�Ȃ����傫���̐����������Ȃ��̂Ŏ���
	*/
	float mLocalScale;
};

