#pragma once
#include "Bullet/Action/IActionDefiner.h"

class BulletCollisionShape;

class TestAction2 :
	public IActionDefiner
{
public:
	TestAction2(std::shared_ptr<BulletCollisionShape> shape, int tag);
	~TestAction2();

	/**
	*	@brief	�Փ˂����ŏ��ɌĂяo�����֐�
	*	@brief	tag		�Փ˂����R���W�����̃^�O
	*/
	void OnAction(int tag1, int tag2);

	/**
	*	@brief	�Փ˂��Ă����OnAction�̌ジ���ƌĂяo�����֐�
	*	@param[in]	tag		�Փ˂����R���W�����̃^�O
	*/
	void StayAction(int tag1, int tag2);

	/**
	*	@brief	�Փ˂��Ȃ��Ȃ������ɔ�яo�����֐�
	*	@param[in]	tag		�Փ˂����R���W�����̃^�O
	*/
	void ExitAction(int tag, int tag2);
};

