#pragma once
#include <Game/Scene/Scene.h>
#include <memory>
#include <DirectXMath.h>
#include <vector>

class GameObject;
class Phase;
class PhaseChanger;

class GameScene :
	public Scene
{
public:
	enum class PrimitiveType
	{
		CUBE,
		SPHERE,
		CAPSULE,
		CONE,
		PLANE
	};

	GameScene(const PrimitiveType type);
	~GameScene();

	/**
	*	@brief	初期化処理
	*/
	void Initialize();

	/**
	*	@brief	終了処理
	*/
	void Terminate();

	/**
	*	@brief	ゲームの更新をする
	*	@param[in]	input	入力
	*/
	void Update(const DxInput& input);

	/**
	*	@brief	描画をする
	*/
	void Draw();
private:
	std::shared_ptr<GameObject> mBase;
	std::shared_ptr<Phase> mPhase;
	std::shared_ptr<PhaseChanger> mChanger;
};