#pragma once
#include <list>
#include <thread>
#include <future>

class AnimationPlayer;



class AnimationPlayerManager
{
public:
	~AnimationPlayerManager();

	void UpdateAnimations();
	static AnimationPlayerManager* Instance();
	int SetAnimation(AnimationPlayer* anim);
	void StopAnimation(int id);
	void ReStartAnimation(int id);
	int RemoveAnimation(int id);
	void WaitSafeFree();

	void StopAllAnimation();
	void ReStartAllAnimation();

	struct AnimatiomInfo
	{
		AnimationPlayer* mAnimPlayer;
		unsigned int id;
		bool isPaly;
	};

private:
	AnimationPlayerManager();

	std::list<std::thread> mThreadObject;

	std::list<AnimatiomInfo>::iterator FindAnimItr(int id);
	int FindMostSmallId();

	std::list<AnimatiomInfo> mAnimations;

	void (AnimationPlayerManager::*mUpdate)();

	void NonUpdate();
	void CommonUpdate();
	bool mIsThreadRun;
};

