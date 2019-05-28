#pragma once
#include "Phase.h"
#include <memory>
#include <DirectXMath.h>

class PhaceChanger;
class GameObject;
class LerpCalculator;

/**
*	@class	�ؒf���ꂽ�v���~�e�B�u������̈ʒu�ֈړ�����t�F�[�Y
*/
class MovePrimitivePhase :
	public Phase
{
public:
	MovePrimitivePhase(PhaseChanger& changer, std::shared_ptr<GameObject>& base
		, std::shared_ptr<GameObject>& plus, std::shared_ptr<GameObject>& minus);
	virtual ~MovePrimitivePhase();

	/**
	*	@brief	�t�F�[�Y�̍X�V
	*/
	void Update(const DxInput& input);

	/**
	*	@brief	�`��
	*/
	void Draw();

	/**
	*	@brief	����������
	*/
	void Initialize();

	/**
	*	@brief	�I������
	*/
	void Terminate();
private:
	std::shared_ptr<GameObject> mBase;
	std::shared_ptr<GameObject> mPlus;
	std::shared_ptr<GameObject> mMinus;

	/**
	*	�J�E���g�p�^�C�}�[
	*/
	unsigned int mFrameCounter;

	/**
	*	�ŏI�ڕW�t���[��
	*/
	unsigned int mGoalFrame;
	DirectX::XMFLOAT3 mFirstPlusVal;
	DirectX::XMFLOAT3 mFirstMinusVal;
	std::shared_ptr<LerpCalculator> mLerp;

	void (MovePrimitivePhase::*mUpdateState)(const DxInput& input);

	/**
	*	@brief	�ؒf���ꂽ��̈ړ��X�V���
	*	@param@[in]	input	���͏��
	*/
	void UpdateCutMove(const DxInput& input);

	/**
	*	@brief	�I����Ԃ̈ʒu�ւ̈ړ��X�V���
	*	@param[in]	input	���͏��
	*/
	void UpdateChoseMove(const DxInput& input);
};

