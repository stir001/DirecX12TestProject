#pragma once
#include <memory>

class Phase;

class PhaseChanger
{
public:
	PhaseChanger();
	~PhaseChanger();

	/**
	*	@brief	次のフェーズを設定する
	*			この関数を読んだ段階では予約するだけ
	*/
	void ChangePhase(const std::shared_ptr<Phase>& phase);

	/**
	*	@brief	フェーズを取得する
	*			次のフェーズが予約されている場合は
	*			現在のフェーズを終了し次のフェーズを返す
	*	@return	フェーズ
	*/
	std::shared_ptr<Phase> GetPhase();
private:
	std::shared_ptr<Phase> mCurrentPhase;
	std::shared_ptr<Phase> mNextPhase;

	std::shared_ptr<Phase>(PhaseChanger::*mGetState)();

	std::shared_ptr<Phase> GetCurrentPhase();
	std::shared_ptr<Phase> GetNextPhase();
};

