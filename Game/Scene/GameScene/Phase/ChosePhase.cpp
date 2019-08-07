#include "ChosePhase.h"
#include "PhaseChanger.h"
#include "WaitCutPhase.h"
#include "Game/Extend/VertexCustumController.h"
#include "Game/GameObject/ScreenRayCast.h"
#include <Game/GameObject/GameObject.h>
#include <Game/Utillity/LerpCalculator.h>
#include <Dx12MSLib.h>

ChosePhase::ChosePhase(PhaseChanger& changer, std::shared_ptr<GameObject> base
	, std::shared_ptr<GameObject> plus, std::shared_ptr<GameObject> minus)
	: Phase(changer), mBase(base), mPlus(plus), mMinus(minus), mUpdateState(nullptr), mDrawState(nullptr)
	, mFrameCounter(0U), mGoalFrame(0U), mLerp(nullptr), mFirstVal{0.0f,0.0f,0.0f}, mCustumCtrl(nullptr)
{
}

ChosePhase::~ChosePhase()
{
}

void ChosePhase::Update(const DxInput & input)
{
	(this->*mUpdateState)(input);
}

void ChosePhase::Draw()
{
	(this->*mDrawState)();
}

void ChosePhase::Initialize()
{
	mUpdateState = &ChosePhase::UpdateChose;
	mDrawState = &ChosePhase::DrawChose;
	mLerp = std::make_unique<LerpCalculator>();
	mCustumCtrl = std::make_unique<VertexCustumController>();
}

void ChosePhase::Terminate()
{
}

void ChosePhase::UpdateChose(const DxInput& input)
{
	if (input.IsKeyTrigger(VIRTUAL_KEY_INDEX::KEY_LBUTTON))
	{
		auto isPlus = IsHitSphare(mPlus, input.GetMousePos(), mPlus->GetCollisionRadius());

		auto isMinus = IsHitSphare(mMinus, input.GetMousePos(), mMinus->GetCollisionRadius());

		if (isPlus && isMinus)
		{
			auto cameraPos = GetCamera()->GetPos();
			auto p = GetLength(mPlus->GetPos() - cameraPos);
			auto m = GetLength(mMinus->GetPos() - cameraPos);
			if (p > m)
			{
				mBase = mMinus;
				ChangeMoveOrigin();
			}
			else
			{
				mBase = mPlus;
				ChangeMoveOrigin();
			}
		}
		else if (isPlus)
		{
			mBase = mPlus;
			ChangeMoveOrigin();
		}
		else if (isMinus)
		{
			mBase = mMinus;
			ChangeMoveOrigin();
		}
	}
}

void ChosePhase::UpdateMoveOrigin(const DxInput& input)
{
	if (mFrameCounter <= mGoalFrame)
	{
		float rate = static_cast<float>(mFrameCounter) / static_cast<float>(mGoalFrame);
		auto pos = mLerp->Lerp(mFirstVal, { 0.0f,0.0f,0.0f }, rate);
		mBase->SetPos(pos);
		++mFrameCounter;
	}
	else
	{
		mBase->SetPos({ 0.0f,0.0f,0.0f });
		auto phase = std::make_shared<WaitCutPhase>(mChanger, mBase);
		mChanger.ChangePhase(phase);
	}
}

void ChosePhase::ChangeMoveOrigin()
{
	mFirstVal = mBase->GetPos();
	mGoalFrame = 30U;
	mFrameCounter = 0U;
	mUpdateState = &ChosePhase::UpdateMoveOrigin;
	mDrawState = &ChosePhase::DrawMoveOrigin;
}

void ChosePhase::SetRayLine(const DxInput & input)
{
	auto nearPos = ScreenRayCast::GetScreenSpace(input.GetMousePos(), 0.0f);
	auto farPos = ScreenRayCast::GetScreenSpace(input.GetMousePos(), 1.0f);

	std::vector<PrimitiveVertex> verts(2);
	verts[0].pos = ConvertToXMFloat4(nearPos);
	verts[1].pos = ConvertToXMFloat4(farPos);

	mCustumCtrl->SetVerts(verts);
}

bool ChosePhase::IsHitTriangle(const std::shared_ptr<GameObject>& obj, const DirectX::XMFLOAT2 & screenPos)
{
	auto& verts = obj->GetVertices();
	auto& indices = obj->GetIndices();
	auto offset = obj->GetPos();

	unsigned int indexNum = static_cast<unsigned int>(indices.size());
	bool rtn = false;
	auto ray = ScreenRayCast::GetRay(screenPos);
	for (unsigned int i = 0; i < indexNum; i += 3)
	{
		auto& v1 = verts[indices[i]];
		auto& v2 = verts[indices[i + 1]];
		auto& v3 = verts[indices[i + 2]];

		if (Dot(ConvertToXMFloat3(v1.normal), ray) > 0)
		{
			continue;
		}

		if (ScreenRayCast::RayCastTriangle(screenPos,
			ConvertToXMFloat3(v1.pos + offset),
			ConvertToXMFloat3(v2.pos + offset),
			ConvertToXMFloat3(v3.pos + offset)))
		{
			rtn = true;
			break;
		}
	}

	return rtn;
}

bool ChosePhase::IsHitSphare(const std::shared_ptr<GameObject>& gameObject, const DirectX::XMFLOAT2 & screenPos, const float radius)
{
	return ScreenRayCast::RayCastSphere(screenPos, gameObject->GetPos(), radius);
}

void ChosePhase::DrawChose()
{
	mPlus->Draw();
	mMinus->Draw();
	mCustumCtrl->Draw();
}

void ChosePhase::DrawMoveOrigin()
{
	mBase->Draw();
}
