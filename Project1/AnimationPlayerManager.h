#pragma once
#include <vector>

class AnimationPlayer;

class AnimationPlayerManager
{
private:
	std::vector<AnimationPlayer*> animations;
	AnimationPlayerManager();
public:
	void UpdateAnimations();
	static AnimationPlayerManager* Instance();
	int SetAnimation(AnimationPlayer* anim);
	void RemoveAnimation(int id);
	~AnimationPlayerManager();
};

