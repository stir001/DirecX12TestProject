#pragma once
#include <memory>

class Phase;

class PhaseChanger
{
public:
	PhaseChanger();
	~PhaseChanger();

	/**
	*	@brief	���̃t�F�[�Y��ݒ肷��
	*			���̊֐���ǂ񂾒i�K�ł͗\�񂷂邾��
	*/
	void ChangePhase(const std::shared_ptr<Phase>& phase);

	/**
	*	@brief	�t�F�[�Y���擾����
	*			���̃t�F�[�Y���\�񂳂�Ă���ꍇ��
	*			���݂̃t�F�[�Y���I�������̃t�F�[�Y��Ԃ�
	*	@return	�t�F�[�Y
	*/
	std::shared_ptr<Phase> GetPhase();
private:
	std::shared_ptr<Phase> mCurrentPhase;
	std::shared_ptr<Phase> mNextPhase;

	std::shared_ptr<Phase>(PhaseChanger::*mGetState)();

	std::shared_ptr<Phase> GetCurrentPhase();
	std::shared_ptr<Phase> GetNextPhase();
};

