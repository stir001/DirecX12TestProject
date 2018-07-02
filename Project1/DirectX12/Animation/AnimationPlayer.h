#pragma once
class AnimationPlayerManager;

class AnimationPlayer
{
	friend AnimationPlayerManager;
public:
	virtual void Update() = 0;
	AnimationPlayer();
	virtual ~AnimationPlayer();
	virtual int GetID() const;
	virtual bool IsEnd() const;
protected:
	int mAnimationId;
	bool mIsLoop;
	bool mIsEnd;
};

