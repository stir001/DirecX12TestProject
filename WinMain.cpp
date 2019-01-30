#include <Dx12MSLib.h>
#include <Windows.h>
#include <vector>

#include "Collision/CollisionChecker.h"
#include "Collision/CapsuleCollider.h"
#include "Collision/SphereCollider.h"
#include "RandomGenerator.h"

using namespace DirectX;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	//Direct3D12‚Ì‰Šú‰»
	Dx12CtrlInit(hInst,"1601295_^“ç§ˆê˜Y");
	
	auto& camera = Dx12Ctrl::Instance().GetCameraHolder()->GetCamera(0);
	DxInput input;
	{
		auto generator = std::make_shared<RandomGenerator>();

		unsigned int capsuleNum = 10;
		std::vector<std::shared_ptr<CapsuleCollider>> capsules;
		capsules.reserve(capsuleNum);

		for (unsigned int i = 0; i < capsuleNum; ++i)
		{
			capsules.emplace_back(std::make_shared<CapsuleCollider>(5.0f, 10.0f));
			capsules.back()->SetPos({ generator->GetValue(-50.0f, 50.0f), 0.0f, generator->GetValue(-50.0f, 100.0f) });
		}

		auto sph = std::make_shared<SphereCollider>(10);

		DirectX::XMFLOAT3 cameraTarget = sph->GetSphereData().origin;
		DirectX::XMFLOAT3 cameraPos = { 0.0f, 30.0f, -70.0f };

		auto checker = std::make_shared<CollisionChecker>();
		float cameraVel = 0.5f;

		while (ProcessMessage()) {
			input.UpdateKeyState();

			sph->DefaultMove(input);

			camera->DefaultMove(input);

			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_W))
			{
				cameraPos.z += cameraVel;
			}
			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_S))
			{
				cameraPos.z -= cameraVel;
			}
			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_D))
			{
				cameraPos.x += cameraVel;
			}
			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_A))
			{
				cameraPos.x -= cameraVel;
			}

			camera->SetPos(cameraPos);
			camera->SetTarget(sph->GetSphereData().origin);
			bool isSphHit = false;
			DirectX::XMFLOAT3 hitcolor = { 0,1.0f,0.0f };
			float resetZLine = -100.0f;

			for (auto& c : capsules)
			{
				c->AddRotateX(generator->GetValue(0.0f, 5.0f));
				c->AddRotateZ(generator->GetValue(0.0f, 5.0f));
				c->AddRotateY(generator->GetValue(0.0f, 5.0f));
				c->AddPos({ 0.0f, 0.0f, -2.0f });
				if (c->GetPos().z < resetZLine)
				{
					c->SetPos({generator->GetValue(-50.0f, 50.0f), 0.0f, 100.0f});
				}
				if (checker->IsCollide(*sph, *c))
				{
					c->SetColor(hitcolor);
					isSphHit = true;
				}
				else
				{
					c->SetColor({ 1.0f,1.0f,1.0f });
				}
				c->Draw();
			}

			if (isSphHit)
			{
				sph->SetColor(hitcolor);
			}
			else
			{
				sph->SetColor({ 1.0f, 1.0f,1.0f });
			}

			//fbx->Draw();
			sph->Draw();

		}

		
	}
	camera = nullptr;
	
	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
}
