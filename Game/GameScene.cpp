#include "GameScene.h"
#include "Dx12MSLib.h"
#include "PrimitiveCutter.h"
#include "FreePrimitive.h"
#include "RandomGenerator.h"
#include <random>

GameScene::GameScene(const PrimitiveType type) : mRandom(std::make_shared<RandomGenerator>(-10.0f,10.0f))
{
	auto& creater = PrimitiveCreator::Instance();
	const float radius = 10.0f;
	const unsigned int div = 20U;
	const float height = 20.0f;
	switch (type)
	{
	case GameScene::PrimitiveType::CUBE:
		mBase = creater.GetCube(radius);
		break;
	case GameScene::PrimitiveType::SPHERE:
		mBase = creater.GetSphere(radius, div);
		break;
	case GameScene::PrimitiveType::CAPSULE:
		mBase = creater.GetCapsule(radius, height, div);
		break;
	case GameScene::PrimitiveType::CONE:
		mBase = creater.GetCone(radius, height, div);
		break;
	case GameScene::PrimitiveType::PLANE:
		mBase = creater.GetPlane(radius, radius, { 0.0f, 1.0f, 0.0f });
		break;
	default:
		break;
	}
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	mBaseCtrl = PrimitiveCreator::Instance().CreateCustumPrimitve(mBase);
	mCutter = std::make_shared<PrimitiveCutter>();
	mDrawState = &GameScene::DrawBase;

	
}

void GameScene::Terminate()
{
	mBaseCtrl.reset();
	mPlusCtrl.reset();
	mMinusCtrl.reset();
	mBase.reset();
	mPlus.reset();
	mMinus.reset();
}

void GameScene::Update(const DxInput & input)
{
	auto camera = GetCamera();
	camera->DefaultMove(input);

	if (input.IsKeyTrigger(VIRTUAL_KEY_INDEX::KEY_ENTER))
	{
		DirectX::XMFLOAT3 normal = { mRandom->Get(),mRandom->Get(),mRandom->Get() };
		normal = NormalizeXMFloat3(normal);
		Cut({ 0.0f,0.0f,0.0f }, normal);
		mDrawState = &GameScene::DrawCut;
	}
}

void GameScene::Draw()
{
	(this->*mDrawState)();
}

void GameScene::Cut(const DirectX::XMFLOAT3& origin, const DirectX::XMFLOAT3& normal)
{
	PrimitiveCutter::CutData data;
	data.face.origin = origin;
	data.face.normal = normal;
	data.primitive.verts = mBase->GetVertices();
	data.primitive.indices = mBase->GetIndices();
	auto cutVerts = mCutter->CutPrimitive(data);

	auto offsetPlus = RePlaceVertsPos(cutVerts.plus.verts);
	auto offsetMinus = RePlaceVertsPos(cutVerts.minus.verts);

	mPlus = std::make_shared<FreePrimitive>(cutVerts.plus.verts, cutVerts.plus.indices);
	mMinus = std::make_shared<FreePrimitive>(cutVerts.minus.verts, cutVerts.minus.indices);

	const float length = 5.0f;

	mPlusCtrl = PrimitiveCreator::Instance().CreateCustumPrimitve(mPlus);
	mPlusCtrl->SetPosition(offsetPlus + NormalizeXMFloat3(offsetPlus) * length);
	mMinusCtrl = PrimitiveCreator::Instance().CreateCustumPrimitve(mMinus);
	mMinusCtrl->SetPosition(offsetMinus + NormalizeXMFloat3(offsetMinus) * length);
}

void GameScene::DrawBase()
{
	mBaseCtrl->Draw();
}

void GameScene::DrawPlus()
{
	mPlusCtrl->Draw();
}

void GameScene::DrawMinus()
{
	mMinusCtrl->Draw();
}

void GameScene::DrawCut()
{
	DrawPlus();
	DrawMinus();
}

DirectX::XMFLOAT3 GameScene::RePlaceVertsPos(std::vector<PrimitiveVertex>& verts)
{
	DirectX::XMFLOAT4 avePos = { 0.0f,0.0f,0.0f,1.0f };
	for (auto& v : verts)
	{
		avePos += v.pos;
	}
	avePos /= static_cast<float>(verts.size());
	for (auto& v : verts)
	{
		v.pos -= avePos;
	}
	return ConvertXMFloat4ToXMFloat3(avePos);
}
