#pragma once

class DxInput;
class PhaseChanger;

/**
*	�Q�[���V�[�����̒i�K�𕪂��邽�߂̊��N���X
*/
class Phase
{
public:
	Phase(PhaseChanger& changer);
	virtual ~Phase();

	/**
	*	@brief	�t�F�[�Y�̍X�V
	*			���t���[���Ă΂��
	*/
	virtual void Update(const DxInput& input) = 0;

	/**
	*	@brief	�`��
	*			���t���[���Ă΂��
	*/
	virtual void Draw() = 0;

	/**
	*	@brief	����������
	*			PhaseChanger�ɂ���ČĂ΂��
	*/
	virtual void Initialize() = 0;

	/**
	*	@brief	�I������
	*			PhaceChanger�ɂ���ČĂ΂��
	*/
	virtual void Terminate() = 0;
protected:
	PhaseChanger& mChanger;
};

