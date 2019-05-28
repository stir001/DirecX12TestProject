#pragma once
#include "Phase.h"
#include <memory>
#include <DirectXMath.h>

class PhaseChanger;
class GameObject;
class LerpCalculator;

class ChosePhase :
	public Phase
{
public:
	ChosePhase(PhaseChanger& changer, std::shared_ptr<GameObject> base
		, std::shared_ptr<GameObject> plus, std::shared_ptr<GameObject> minus);
	virtual ~ChosePhase();
	/**
	*	@brief	�t�F�[�Y�̍X�V
	*			���t���[���Ă΂��
	*/
	 void Update(const DxInput& input);

	/**
	*	@brief	�`��
	*			���t���[���Ă΂��
	*/
	 void Draw();

	/**
	*	@brief	����������
	*			PhaseChanger�ɂ���ČĂ΂��
	*/
	 void Initialize();

	/**
	*	@brief	�I������
	*			PhaceChanger�ɂ���ČĂ΂��
	*/
	 void Terminate();
private:
	std::shared_ptr<GameObject> mBase;
	std::shared_ptr<GameObject> mPlus;
	std::shared_ptr<GameObject> mMinus;
	std::unique_ptr<LerpCalculator> mLerp;
	DirectX::XMFLOAT3 mFirstVal;
	unsigned int mFrameCounter;
	unsigned int mGoalFrame;

	void (ChosePhase::*mUpdateState)(const DxInput&);
	void (ChosePhase::*mDrawState)();

	void UpdateChose(const DxInput& input);
	void UpdateMoveOrigin(const DxInput& input);
	void ChangeMoveOrigin();

	void DrawChose();
	void DrawMoveOrigin();
};

