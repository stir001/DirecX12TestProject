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
		data = creater.GetSphere(radius / 2.0f, div);
		break;
	case GameScene::PrimitiveType::CAPSULE:
		data = creater.GetCapsule(radius / 3.0f, height / 1.5f, div);
		break;
	case GameScene::PrimitiveType::CONE:
		data = creater.GetCone(radius / 1.5f, height / 1.5f, div);
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
	MoveCamera(input);
	mPhase->Update(input);
}

void GameScene::Draw()
{
	mPhase->Draw();
}

void GameScene::MoveCamera(const DxInput & input)
{
	if (input.IsKeyTrigger(VIRTUAL_KEY_INDEX::KEY_RBUTTON))
	{
		mMousePos = input.GetMousePos();
	}
	else if (input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_RBUTTON))
	{
		auto camera = GetCamera();
		auto cameraPos = camera->GetPos();
		auto target = camera->GetTaregt();

		auto mousePos = input.GetMousePos();
		auto mouseVec = mousePos - mMousePos;
		DirectX::XMFLOAT3 upper = { 0.0f,1.0f,0.0f };

		auto rad = Dot(upper, Normalize(target - cameraPos));

		auto size = Dx12Ctrl::Instance().GetWindowSize();

		auto bigger = (size.x > size.y ? size.x : size.y) / 2.0f;

		auto unit = DirectX::XMFLOAT2(bigger, bigger);
		float pi = 3.14159265f;
		auto yAxisRota = mouseVec.x / unit.x * 2.0f * pi;
		auto sideAxisRota = -mouseVec.y / unit.y * 2.0f * pi;

		auto yMatrix = ConvertToXMFloat4x4(DirectX::XMMatrixRotationY(yAxisRota));
		auto rotaAxis = Normalize(Cross(-Normalize(ConvertToXMFloat3(cameraPos)), { 0.0f,1.0f,0.0f }));
		auto rotaMat = CreateQuoternion(rotaAxis, sideAxisRota);
		
		cameraPos *= yMatrix * rotaMat;
		camera->SetPos(cameraPos.x, cameraPos.y, cameraPos.z);
		camera->SetTarget(0.0f, 0.0f, 0.0f);
		mMousePos = mousePos;
	}
}
