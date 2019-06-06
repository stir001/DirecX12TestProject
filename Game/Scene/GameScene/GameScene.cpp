#include "GameScene.h"
#include <Game/GameObject/GameObject.h>
#include "Phase/Phase.h"
#include "Phase/WaitCutPhase.h"
#include "Phase/PhaseChanger.h"
#include <Dx12MSLib.h>

GameScene::GameScene(const PrimitiveType type) 
	: mPhase(nullptr), mBase(nullptr)
{
	auto& creater = PrimitiveCreator::Instance();
	const float radius = 10.0f;
	const unsigned int div = 20U;
	const float height = 20.0f;
	std::shared_ptr<PrimitiveObject> data;
	switch (type)
	{
	case GameScene::PrimitiveType::CUBE:
		data = creater.GetCube(radius);
		break;
	case GameScene::PrimitiveType::SPHERE:
		data = creater.GetSphere(radius, div);
		break;
	case GameScene::PrimitiveType::CAPSULE:
		data = creater.GetCapsule(radius / 2.0f, height, div);
		break;
	case GameScene::PrimitiveType::CONE:
		data = creater.GetCone(radius, height, div);
		break;
	case GameScene::PrimitiveType::PLANE:
		data = creater.GetPlane(radius, radius, { 0.0f, 1.0f, 0.0f });
		break;
	default:
		break;
	}
	mBase = std::make_shared<GameObject>(PrimitiveCreator::Instance().CreateCustumPrimitve(data), data);
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	mChanger = std::make_shared<PhaseChanger>();
	mPhase = std::make_shared<WaitCutPhase>(*mChanger,mBase);
	mChanger->ChangePhase(mPhase);
}

void GameScene::Terminate()
{
}

void GameScene::Update(const DxInput & input)
{
	mPhase = mChanger->GetPhase();
	mPhase->Update(input);
}

void GameScene::Draw()
{
	mPhase->Draw();
}
