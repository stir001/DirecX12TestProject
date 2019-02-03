#include "MoveRestrictionApp.h"
#include "PlayerCharacter.h"
#include "MoveRestrictionMesh.h"
#include "PlaneCollisionCheckerXZ.h"
#include "TriangleDef.h"
#include <Dx12MSLib.h>


MoveRestrictionApp::MoveRestrictionApp()
{
}


MoveRestrictionApp::~MoveRestrictionApp()
{
}

void MoveRestrictionApp::Run()
{
	auto& camera = Dx12Ctrl::Instance().GetCameraHolder()->GetCamera(0);
	DxInput input;
	{
		auto player = std::make_shared<PlayerCharacter>(camera, 10.0f);
		unsigned int div = 20;
		auto custumMesh = std::make_shared<MoveRestrictionMesh>(div);
		auto checker = std::make_shared<PlaneCollisionCheckerXZ>();
		float unitRad = (3.14159265f * 2.0f) / static_cast<float>(div);
		while (ProcessMessage()) {
			input.UpdateKeyState();

			player->Move(input);

			auto pos = player->GetPos();
			unsigned int num = custumMesh->GetInRangePolygonNum();
			for (unsigned int i = 0; i < num; ++i)
			{
				PRIMITIVE::Triangle tri = custumMesh->GetInRangePolygon(i);
				if (checker->IsInTriangle(pos, tri))
				{
					auto p2Top3 = tri.pos3 - tri.pos2;
					auto p2ToPos = pos - tri.pos2;
					auto length = sqrtf(p2Top3.x * p2Top3.x + p2Top3.z * p2Top3.z);
					auto normVec = DirectX::XMFLOAT2(p2Top3.x / length, p2Top3.z / length);
					auto p2PosVec = DirectX::XMFLOAT2(p2ToPos.x, p2ToPos.z);
					auto proj = p2PosVec.x * normVec.x + p2PosVec.y * normVec.y;
					auto projVec = DirectX::XMFLOAT2(normVec.x * proj, normVec.y * proj);
					auto offset = DirectX::XMFLOAT3(projVec.x - p2PosVec.x, pos.y, projVec.y - p2PosVec.y);
					pos += offset;
					player->SetPosition(pos);
				};
			}

			{
				DirectX::XMFLOAT2 pivotVec = { 0.0f, 1.0f };
				DirectX::XMFLOAT2 posVec = { pos.x, pos.z };
				//float cosValue = DotXMFloat2(NormalizeXMFloat2(posVec), pivotVec);
				auto theta = atan2f(posVec.x , posVec.y);

				if (theta < 0)
				{
					theta += (3.14159265f * 2.0f);
				}

				PRIMITIVE::Triangle tri = custumMesh->GetOutRangePolygon(static_cast<unsigned int>(theta / (3.14159265f * 2.0f) * div));
				if (!checker->IsInTriangle(pos, tri))
				{
					auto p2Top3 = tri.pos3 - tri.pos2;
					auto p2ToPos = pos - tri.pos2;
					auto length = sqrtf(p2Top3.x * p2Top3.x + p2Top3.z * p2Top3.z);
					auto normVec = DirectX::XMFLOAT2(p2Top3.x / length, p2Top3.z / length);
					auto p2PosVec = DirectX::XMFLOAT2(p2ToPos.x, p2ToPos.z);
					auto proj = p2PosVec.x * normVec.x + p2PosVec.y * normVec.y;
					auto projVec = DirectX::XMFLOAT2(normVec.x * proj, normVec.y * proj);
					auto offset = DirectX::XMFLOAT3(projVec.x - p2PosVec.x, pos.y, projVec.y - p2PosVec.y);
					pos += offset;
					player->SetPosition(pos);
				};
			}


			player->Draw();



			custumMesh->Draw();

		}
	}
	camera = nullptr;

	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
}

void MoveRestrictionApp::Initialize(HINSTANCE hInst)
{
#ifndef _DEBUG
	ShaderCompiler::Instance().SetShaderDirPath("Shader/");
#endif
	Dx12CtrlInit(hInst, "1601295_ê^ìÁèßàÍòY");
}
