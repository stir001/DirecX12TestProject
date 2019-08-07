#include "FreeCutScene.h"
#include "Game/GameObject/ScreenRayCast.h"
#include "Game/GameObject/PrimitiveCutter.h"
#include "Game/GameObject/GameObject.h"
#include "Game/GameObject/FreePrimitive.h"
#include <Dx12MSLib.h>


FreeCutScene::FreeCutScene(int type)
{
	auto& creater = PrimitiveCreator::Instance();
	std::shared_ptr<PrimitiveObject> data;
	const float radius = 10.0f;
	const unsigned int div = 20U;
	const float height = 20.0f;
	switch (static_cast<PrimitiveType>(type))
	{
	case FreeCutScene::PrimitiveType::CUBE:
		data = creater.GetCube(radius);
		break;
	case FreeCutScene::PrimitiveType::SPHERE:
		data = creater.GetSphere(radius / 2.0f, div);
		break;
	case FreeCutScene::PrimitiveType::CAPSULE:
		data = creater.GetCapsule(radius / 3.0f, height / 1.5f, div);
		break;
	case FreeCutScene::PrimitiveType::CONE:
		data = creater.GetCone(radius / 1.5f, height / 1.5f, div);
		break;
	default:
		break;
	}

	auto obj = std::make_shared<GameObject>(PrimitiveCreator::Instance().CreateCustumPrimitve(data), data);
	mObjects.push_back(obj);
}

FreeCutScene::~FreeCutScene()
{
}

void FreeCutScene::Initialize()
{
	mCutter = std::make_shared<PrimitiveCutter>();
	mUpdateState = &FreeCutScene::WaitCutStart;
}

void FreeCutScene::Terminate()
{
}

void FreeCutScene::Update(const DxInput & input)
{
	MoveCamera(input);
	for (auto& obj : mObjects)
	{
		obj->Update();
	}
	(this->*mUpdateState)(input);
}

void FreeCutScene::Draw()
{
	for (auto& o : mObjects)
	{
		o->Draw();
	}
}
void FreeCutScene::MoveCamera(const DxInput & input)
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

void FreeCutScene::WaitCutStart(const DxInput & input)
{
	if (input.IsKeyTrigger(VIRTUAL_KEY_INDEX::KEY_LBUTTON))
	{
		auto mouse = input.GetMousePos();
		mStart = ScreenRayCast::GetScreenSpace(mouse);
		mUpdateState = &FreeCutScene::ChoseCutEnd;
	}
}

void FreeCutScene::ChoseCutEnd(const DxInput & input)
{
	if (input.IsKeyUp(VIRTUAL_KEY_INDEX::KEY_LBUTTON))
	{
		auto mouse = input.GetMousePos();
		mEnd = ScreenRayCast::GetScreenSpace(mouse);

		if (GetLength(mEnd - mStart) > 0.1f)
		{
			Cut();
		}
		mUpdateState = &FreeCutScene::WaitCutStart;
	}
}

void FreeCutScene::Cut()
{
	auto cameraPos = ConvertToXMFloat3(GetCamera()->GetPos());
	auto normal = Normalize(Cross(mStart - cameraPos, mEnd - cameraPos));
	PrimitiveCutter::CutData data;
	data.face.normal = normal;
	const unsigned int roopNum = static_cast<unsigned int>(mObjects.size());
	unsigned int roop = 0U;
	
	for (auto o = mObjects.begin(); roop < roopNum && o != mObjects.end(); ++roop)
	{
		auto& obj = *o;
		if (!IsHitSpherePlane(obj->GetPos(), obj->GetCollisionRadius(), cameraPos - obj->GetPos(), normal))
		{
			++o;
			continue;
		}

		data.face.origin = cameraPos - obj->GetPos();
		data.primitive.indices = obj->GetIndices();
		data.primitive.verts = obj->GetVertices();
		auto cutVerts = mCutter->CutPrimitive(data);

		if (cutVerts.minus.verts.size() == 0 || cutVerts.plus.verts.size() == 0)
		{
			++o;
			continue;
		}
		auto offsetPlus = RePlaceVertsPos(cutVerts.plus.verts);
		auto offsetMinus = RePlaceVertsPos(cutVerts.minus.verts);

		auto plusData = std::make_shared<FreePrimitive>(cutVerts.plus.verts, cutVerts.plus.indices);
		auto minusData = std::make_shared<FreePrimitive>(cutVerts.minus.verts, cutVerts.minus.indices);

		auto plus = std::make_shared<GameObject>(PrimitiveCreator::Instance().CreateCustumPrimitve(plusData), plusData);
		auto minus = std::make_shared<GameObject>(PrimitiveCreator::Instance().CreateCustumPrimitve(minusData), minusData);

		auto offset = (*o)->GetPos();
		plus->SetPos(offsetPlus + offset);
		plus->SetVelocity(normal * 0.01f);
		minus->SetPos(offsetMinus + offset);
		minus->SetVelocity(-normal * 0.01f);
		mObjects.emplace_back(plus);
		mObjects.emplace_back(minus);

		o = mObjects.erase(o);
		if (o == mObjects.end())
		{
			break;
		}
	}
}

DirectX::XMFLOAT3 FreeCutScene::RePlaceVertsPos(std::vector<PrimitiveVertex>& verts)
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

bool FreeCutScene::IsHitSpherePlane(const DirectX::XMFLOAT3 & sphOrigin, const float radius, const DirectX::XMFLOAT3 & plOrigin, const DirectX::XMFLOAT3 normal)
{
	return std::fabsf(Dot(sphOrigin, normal) - Dot(plOrigin, normal)) < radius;
}
