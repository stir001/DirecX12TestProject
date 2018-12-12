#pragma once
#include "Bullet/Action/IActionDefiner.h"

class BulletCollisionShape;

class TestAction2 :
	public IActionDefiner
{
public:
	TestAction2(std::shared_ptr<BulletCollisionShape> shape, int tag);
	~TestAction2();

	/**
	*	@brief	衝突した最初に呼び出される関数
	*	@brief	tag		衝突したコリジョンのタグ
	*/
	void OnAction(int tag1, int tag2);

	/**
	*	@brief	衝突している間OnActionの後ずっと呼び出される関数
	*	@param[in]	tag		衝突したコリジョンのタグ
	*/
	void StayAction(int tag1, int tag2);

	/**
	*	@brief	衝突しなくなった時に飛び出される関数
	*	@param[in]	tag		衝突したコリジョンのタグ
	*/
	void ExitAction(int tag, int tag2);
};

