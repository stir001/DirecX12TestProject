#include "AnimationPlayerManager.h"
#include "AnimationPlayer.h"


AnimationPlayerManager::AnimationPlayerManager()
{
}


AnimationPlayerManager::~AnimationPlayerManager()
{
}

AnimationPlayerManager* AnimationPlayerManager::Instance()
{
	static AnimationPlayerManager* animplayer = nullptr;
	if (animplayer == nullptr)
	{
		animplayer = new AnimationPlayerManager();
	}
	return animplayer;
}

int AnimationPlayerManager::SetAnimation(AnimationPlayer* anim)
{
	animations.push_back(anim);
	return (animations.size() - 1);
}

void AnimationPlayerManager::UpdateAnimations()
{
	for (auto& anim : animations)
	{
		anim->Update();
	}
}

void AnimationPlayerManager::RemoveAnimation(int id)
{
	animations.erase((animations.begin() + id));
}