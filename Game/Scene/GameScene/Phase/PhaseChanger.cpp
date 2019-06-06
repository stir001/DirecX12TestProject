#include "PhaseChanger.h"
#include "Phase.h"


PhaseChanger::PhaseChanger(): mGetState(&PhaseChanger::GetCurrentPhase)
	, mCurrentPhase(nullptr), mNextPhase(nullptr)
{
}

PhaseChanger::~PhaseChanger()
{
}

void PhaseChanger::ChangePhase(const std::shared_ptr<Phase>& phase)
{
	mNextPhase = phase;
	mGetState = &PhaseChanger::GetNextPhase;
}

std::shared_ptr<Phase> PhaseChanger::GetPhase()
{
	return (this->*mGetState)();
}

std::shared_ptr<Phase> PhaseChanger::GetCurrentPhase()
{
	return mCurrentPhase;
}

std::shared_ptr<Phase> PhaseChanger::GetNextPhase()
{
	if (mCurrentPhase != nullptr)
	{
		mCurrentPhase->Terminate();
	}
	mNextPhase->Initialize();
	mCurrentPhase = mNextPhase;
	mNextPhase = nullptr;
	mGetState = &PhaseChanger::GetCurrentPhase;
	return mCurrentPhase;
}
