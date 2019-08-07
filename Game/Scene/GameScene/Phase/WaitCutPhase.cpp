
#include "WaitCutPhase.h"
#include <Game/GameObject/PrimitiveCutter.h>
#include <Game/GameObject/FreePrimitive.h>
#include <Game/GameObject/GameObject.h>
#include <Game/Utillity/RandomGenerator.h>
#include "PhaseChanger.h"
#include "MovePrimitivePhase.h"
#include <Game/GameObject/AxisViewer.h>
#include <Dx12MSLib.h>
#include <Game/Extend/VertexCustumController.h>
#include "Game/GameObject/ScreenRayCast.h"


WaitCutPhase::WaitCutPhase(PhaseChanger& changer,std::shared_ptr<GameObject>& baseObj)
	: Phase(changer), mBase(baseObj), mCutter(std::make_unique<PrimitiveCutter>())
	, mPlus(nullptr), mMinus(nullptr), mAxisViewer(std::make_unique<AxisViewer>())
	, mCustumController(std::make_unique<VertexCustumController>())
	, mNormal{ 1.0f, 0.0f, 0.0f}, mOrigin{ 0.0f, 0.0f, 0.0f}
	, mUpdateState(&WaitCutPhase::UpdateCatchAxis)
{
	Cut(mOrigin, mNormal);
	float lineScale = 1.0025f;
	mCustumController->SetScale(lineScale);
}


WaitCutPhase::~WaitCutPhase()
{
}

void WaitCutPhase::Update(const DxInput & input)
{
	(this->*mUpdateState)(input);
}

void WaitCutPhase::Draw()
{
	mBase->Draw();
	mCustumController->Draw();
	mAxisViewer->Draw();
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

	mPlus = std::make_shared<GameObject>(PrimitiveCreator::Instance().CreateCustumPrimitve(plusData), plusData);
	mMinus = std::make_shared<GameObject>(PrimitiveCreator::Instance().CreateCustumPrimitve(minusData), minusData);

	mPlus->SetPos(offsetPlus);
	mMinus->SetPos(offsetMinus);

	auto vert = mCutter->GetCutLineVertex();
	unsigned int vertNum = static_cast<unsigned int>(vert.size());
	mCustumController->SetVerts(mCutter->GetCutLineVertex());

}

DirectX::XMFLOAT3 WaitCutPhase::RePlaceVertsPos(std::vector<PrimitiveVertex>& verts)
{
	DirectX::XMFLOAT4 maxPos = { FLT_MIN, FLT_MIN, FLT_MIN, 1.0f };
	DirectX::XMFLOAT4 minPos = { FLT_MAX, FLT_MAX, FLT_MAX, 1.0f };
	for (auto& v : verts)
	{
		maxPos.x = maxPos.x <= v.pos.x ? v.pos.x : maxPos.x;
		maxPos.y = maxPos.y <= v.pos.y ? v.pos.y : maxPos.y;
		maxPos.z = maxPos.z <= v.pos.z ? v.pos.z : maxPos.z;
		minPos.x = minPos.x >= v.pos.x ? v.pos.x : minPos.x;
		minPos.y = minPos.y >= v.pos.y ? v.pos.y : minPos.y;
		minPos.z = minPos.z >= v.pos.z ? v.pos.z : minPos.z;
	}
	
	auto center = (maxPos + minPos) / 2.0f;

	for (auto& v : verts)
	{
		v.pos -= center;
	}
	return ConvertToXMFloat3(center);
}

void WaitCutPhase::ChoseNormal(const DxInput& input)
{
}

void WaitCutPhase::UpdateRotaAxis(const DxInput& input)
{
	if (input.IsKeyUp(VIRTUAL_KEY_INDEX::KEY_LBUTTON))
	{
		mAxisViewer->ReleaseUI();
		mUpdateState = &WaitCutPhase::UpdateCatchAxis;
		return;
	}

	auto mousePos = input.GetMousePos();
	DirectX::XMFLOAT3 origin = { 0.0f,0.0f,0.0f };

	auto hit = ScreenRayCast::RayCastPlane(mousePos, origin, mRotaAxis).hitPos;

	auto hitVec = Normalize(hit - origin);
	auto preVec = Normalize(mHitPos - origin);

	if (abs(Dot(hitVec, preVec)) > 0.99f)
	{
		return;
	}

	auto div = 16.0f;
	auto piDiv = DirectX::XM_PI / div;

	auto rad = acosf(Dot(preVec, hitVec));

	rad = piDiv * static_cast<float>(static_cast<int>((rad / piDiv)));
	if (rad == 0.0f)
	{
		return;
	}

	auto axis = Normalize(Cross(preVec, hitVec));
	auto mat = CreateQuoternion(axis, rad);

	mNormal *= mat;
	mHitPos = hit;
	Cut(origin, mNormal);
}

void WaitCutPhase::UpdateCatchAxis(const DxInput & input)
{
	if (input.IsKeyTrigger(VIRTUAL_KEY_INDEX::KEY_LBUTTON))
	{
		auto axis = mAxisViewer->CatchAxisUI(input.GetMousePos());
		if (axis != AxisViewer::HoldAxis::NONE)
		{
			switch (axis)
			{
			case AxisViewer::HoldAxis::NONE:
				return;
				break;
			case AxisViewer::HoldAxis::X:
				mRotaAxis = { 1.0f,0.0f,0.0f };
				break;
			case AxisViewer::HoldAxis::Y:
				mRotaAxis = { 0.0f,1.0f,0.0f };
				break;
			case AxisViewer::HoldAxis::Z:
				mRotaAxis = { 0.0f,0.0f,1.0f };
				break;
			default:
				return;
				break;
			}
			mHitPos = mAxisViewer->GetRayHitPos();
			mUpdateState = &WaitCutPhase::UpdateRotaAxis;
		}
	}

	if (input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_LBUTTON) && input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_RBUTTON))
	{
		auto normal = mNormal;
		mPlus->SetVelocity(normal);
		mMinus->SetVelocity(-normal);
		auto phase = std::make_shared<MovePrimitivePhase>(mChanger, mBase, mPlus, mMinus);
		mChanger.ChangePhase(phase);
	}
}
