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
	*	@brief	����������
	*/
	void Initialize();

	/**
	*	@brief	�I������
	*/
	void Terminate();

	/**
	*	@brief	�Q�[���̍X�V������
	*	@param[in]	input	����
	*/
	void Update(const DxInput& input);

	/**
	*	@brief	�`�������
	*/
	void Draw();
private:
	std::shared_ptr<GameObject> mBase;
	std::shared_ptr<Phase> mPhase;
	std::shared_ptr<PhaseChanger> mChanger;
};