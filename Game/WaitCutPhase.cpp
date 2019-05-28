#include "WaitCutPhase.h"
#include "PrimitiveCutter.h"
#include "FreePrimitive.h"
#include "GameObject.h"
#include "RandomGenerator.h"
#include "PhaseChanger.h"
#include "MovePrimitivePhase.h"
#include <Dx12MSLib.h>


WaitCutPhase::WaitCutPhase(PhaseChanger& changer,std::shared_ptr<GameObject>& baseObj)
	: Phase(changer), mBase(baseObj), mCutter(std::make_unique<PrimitiveCutter>())
	, mPlus(nullptr), mMinus(nullptr)
{
}


WaitCutPhase::~WaitCutPhase()
{
}

void WaitCutPhase::Update(const DxInput & input)
{
	auto camera = GetCamera();
	//camera->DefaultMove(input);

	if (input.IsKeyTrigger(VIRTUAL_KEY_INDEX::KEY_ENTER))
	{
		auto[origin, normal] = GetCutFace();
		Cut(origin, normal);
		mPlus->SetVelocity(normal);
		mMinus->SetVelocity(-normal);
		auto phase = std::make_shared<MovePrimitivePhase>(mChanger, mBase, mPlus, mMinus);
		mChanger.ChangePhase(phase);
	}
}

void WaitCutPhase::Draw()
{
	mBase->Draw();
}

void WaitCutPhase::Initialize()
{
}

void WaitCutPhase::Terminate()
{
}

void WaitCutPhase::Cut(const DirectX::XMFLOAT3& origin, const DirectX::XMFLOAT3 & normal)
{
	PrimitiveCutter::CutData data;
	data.face.origin = origin;
	data.face.normal = normal;
	data.primitive.verts = mBase->GetVertices();
	data.primitive.indices = mBase->GetIndices();
	auto cutVerts = mCutter->CutPrimitive(data);

	auto offsetPlus = RePlaceVertsPos(cutVerts.plus.verts);
	auto offsetMinus = RePlaceVertsPos(cutVerts.minus.verts);

	auto plusData = std::make_shared<FreePrimitive>(cutVerts.plus.verts, cutVerts.plus.indices);
	auto minusData = std::make_shared<FreePrimitive>(cutVerts.minus.verts, cutVerts.minus.indices);

	const float length = 5.0f;

	mPlus = std::make_shared<GameObject>(PrimitiveCreator::Instance().CreateCustumPrimitve(plusData), plusData);
	mMinus = std::make_shared<GameObject>(PrimitiveCreator::Instance().CreateCustumPrimitve(minusData), minusData);

	mPlus->SetPos(offsetPlus);
	mMinus->SetPos(offsetMinus);
}

DirectX::XMFLOAT3 WaitCutPhase::RePlaceVertsPos(std::vector<PrimitiveVertex>& verts)
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

std::tuple<DirectX::XMFLOAT3, DirectX::XMFLOAT3> WaitCutPhase::GetCutFace() const
{
	//TODO:‰¼‚Ìƒ‰ƒ“ƒ_ƒ€ŽÀ‘•‚È‚Ì‚Å“ü—Í‚©‚ç’l‚ð“¾‚é‚æ‚¤‚É‚·‚é‚±‚Æ
	auto random = RandomGenerator(-1.0f, 1.0f);
	DirectX::XMFLOAT3 normal = { random.Get(),random.Get(),random.Get() };
	normal = NormalizeXMFloat3(normal);
	auto origin = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	return { origin, normal };
}
