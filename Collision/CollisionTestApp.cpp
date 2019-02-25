#include "CollisionTestApp.h"
#include "../RandomGenerator.h"
#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "CollisionChecker.h"

#include <Dx12MSLib.h>


CollisionTestApp::CollisionTestApp()
{
}


CollisionTestApp::~CollisionTestApp()
{
}

void CollisionTestApp::Initialize(HINSTANCE hInst)
{
#ifndef _DEBUG
	ShaderCompiler::Instance().SetShaderDirPath("Shader/");
#endif
	Dx12CtrlInit(hInst, "1601295_^“ç§ˆê˜Y");
}

void CollisionTestApp::Run()
{
	auto& camera = Dx12Ctrl::Instance().GetCameraHolder()->GetCamera(0);
	DxInput input;
	{
		float xRenge = 50.0f;
		float zRange = 200.0f;

		auto generator = std::make_shared<RandomGenerator>();

		unsigned int capsuleNum = 5;
		std::vector<std::shared_ptr<CapsuleCollider>> capsules;
		capsules.reserve(capsuleNum);

		for (unsigned int i = 0; i < capsuleNum; ++i)
		{
			capsules.emplace_back(std::make_shared<CapsuleCollider>(5.0f, 10.0f));
			capsules.back()->SetPos({ generator->GetValue(-xRenge, xRenge), 0.0f, generator->GetValue(-zRange, zRange) });
		}

		unsigned int sphereNum = 5;
		std::vector<std::shared_ptr<SphereCollider>> spheres;
		spheres.reserve(sphereNum);
		for (unsigned int i = 0; i < sphereNum; ++i)
		{
			spheres.emplace_back(std::make_shared<SphereCollider>(5.0f));
			spheres.back()->SetPos({ generator->GetValue(-xRenge, xRenge), 0.0f, generator->GetValue(-zRange, zRange) });
		}

		auto sph = std::make_shared<SphereCollider>(10.0f);

		DirectX::XMFLOAT3 cameraTarget = sph->GetSphereData().origin;
		DirectX::XMFLOAT3 cameraPos = { 0.0f, 30.0f, -70.0f };

		auto checker = std::make_shared<CollisionChecker>();
		float cameraVel = 0.5f;

		while (ProcessMessage()) {
			input.UpdateKeyState();

			sph->DefaultMove(input);

			camera->DefaultMove(input);

			if (input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_W))
			{
				cameraPos.z += cameraVel;
			}
			if (input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_S))
			{
				cameraPos.z -= cameraVel;
			}
			if (input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_D))
			{
				cameraPos.x += cameraVel;
			}
			if (input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_A))
			{
				cameraPos.x -= cameraVel;
			}

			camera->SetPos(cameraPos);
			camera->SetTarget(sph->GetSphereData().origin);
			bool isSphHit = false;
			DirectX::XMFLOAT3 hitcolor = { 0,1.0f,0.0f };
			float resetZLine = -zRange;

			for (auto& c : capsules)
			{
				c->AddRotateX(generator->GetValue(0.0f, 5.0f));
				//c->AddRotateZ(generator->GetValue(0.0f, 5.0f));
				c->AddRotateY(generator->GetValue(0.0f, 5.0f));
				c->AddPos({ 0.0f, 0.0f, -1.5f });
				if (c->GetPos().z < resetZLine)
				{
					c->SetPos({ generator->GetValue(-xRenge, xRenge), 0.0f, zRange / 2.0f });
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

			for (auto& s : spheres)
			{
				s->AddPos({ 0.0f, 0.0f, -2.0f });
				if (s->GetPos().z < resetZLine)
				{
					s->SetPos({ generator->GetValue(-xRenge, xRenge), 0.0f, zRange / 2.0f });
				}
				if (checker->IsCollide(*sph, *s))
				{
					s->SetColor(hitcolor);
					isSphHit = true;
				}
				else
				{
					s->SetColor({ 1.0f,1.0f,1.0f });
				}
				s->Draw();
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
