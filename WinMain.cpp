#include "Dx12MSLib.h"
#include "BulletInclude.h"
#include "sample/NormalMapCube.h"

#include <Windows.h>
#include <DirectXMath.h>

using namespace DirectX;

const std::string FBX_MODEL_DIR = "3DModel/FBX/";

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	//Direct3D12‚Ì‰Šú‰»
	Dx12CtrlInit(hInst, "1601295_^“ç§ˆê˜Y");
	
	auto& camera = Dx12Ctrl::Instance().GetCameraHolder()->GetCamera(0);
	DxInput input;
	{

		//FbxLoader::Create();
		//auto modelData = FbxLoader::Instance()->LoadMesh(FBX_MODEL_PATH2);

		//PrimitiveCreator mgr;


		std::shared_ptr<PMDLoader> loader(new PMDLoader());
		std::shared_ptr<PMDController> pmdContrl = loader->Load("3DModel/PMD/”Ž—í—ì–²/reimu_F01.pmd");
		std::shared_ptr<DirectionalLight> dirLight(new DirectionalLight(1, -1, 1));
		VMDLoader vmdloader;
		//std::shared_ptr<VMDMotion> motion = vmdloader.LoadMotion("”Ž—í—ì–²/reimu_walk.vmd");
		//pmdContrl->SetMotion(motion);
		//pmdContrl->PlayMotion(true);

		//PrimitiveCreator priCreater;

		float length = 20.f;
		std::shared_ptr<PrimitiveController> primitiveCtrl = PrimitiveCreator::Instance().CreateCube(length, "img/cube_tex.png");

		unsigned int xNum = 100;
		unsigned int zNum = 100;
		unsigned int yNum = 10;
		std::vector<DirectX::XMFLOAT3> offsets(xNum * zNum * yNum);
		for (int y = 0; y < yNum; ++y)
		{
			for (int z = 0; z < zNum; ++z)
			{
				for (int x = 0; x < xNum; ++x)
				{
					offsets[x + z * xNum + y * xNum * zNum] = DirectX::XMFLOAT3(x * length, -y * length, z * length);
				}
			}
		}
		float deg = -1.0f;

		primitiveCtrl->Instancing(offsets);
		for (unsigned int i = 0; i < xNum * zNum * yNum; ++i)
		{
			primitiveCtrl->SetColor({ 0.0f,0.0f,0.0f,1.0f }, i);
		}

		pmdContrl->SetLight(dirLight);

		XMFLOAT3 pos = { 0,0,0 };
		XMFLOAT3 normal = { 0,1,0 };

		Transform3DCalculator calculator;

		XMFLOAT4X4 matrix = IdentityXMFloat4x4();

		std::vector<DirectX::XMFLOAT4X4> instanceMatrix(xNum * zNum);

		auto& camera = Dx12Ctrl::Instance().GetCamera(0);
		DxInput input;

		DirectX::XMFLOAT3 zVec = { 0, 0, 1 };
		DirectX::XMFLOAT3 xVec = { 1, 0, 0 };
		DirectX::XMFLOAT3 vel = { 0 ,0, 0 };
		float velLength = 2.0f;
		DirectX::XMFLOAT3 pmdPos = { 0, 0, 0 };
		DirectX::XMFLOAT3 initDir = { 0, 0, -1 };
		float zvecRota = 90;//-z
		float xvecRota = 180;//-x

		float modelrota = 0;
		unsigned int rotaCount = 0;

		while (ProcessMessage()) {
			input.UpdateKeyState();

			camera->DefaultMove(input);

			calculator.AddPositon(DirectX::XMFLOAT3(0, length * 0.5f, length * 0.5f));
			calculator.AddRotaQuaternion(CreateQuoternionXMFloat4(DirectX::XMFLOAT3(1, 0, 0), deg));
			calculator.AddPositon(DirectX::XMFLOAT3(0, -length * 0.5f, -length * 0.5f));
			matrix = calculator.GetAMatrix();
			for (auto& mat : instanceMatrix)
			{
				mat = matrix;
			}
			primitiveCtrl->SetInstancingMatrix(instanceMatrix, 0, xNum * zNum - 1);

			vel = { 0,0,0 };
			modelrota = 0.0f;
			rotaCount = 0;

			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD4))
			{
				vel += -xVec;
			}

			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD8))
			{
				vel += zVec;
			}

			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD6))
			{
				vel += xVec;
			}

			if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD2))
			{
				vel += -zVec;
			}

			if (vel != DirectX::XMFLOAT3(0, 0, 0))
			{
				vel = NormalizeXMFloat3(vel);
				float c = DotXMFloat3(vel, initDir);
				float rad = 0.0f;
				if (vel.x > 0)
				{
					rad = -acosf(c);
				}
				else
				{
					rad = acosf(c);
				}
				pmdPos += vel * velLength;

				pmdContrl->SetRotaQuaternion(CreateQuoternionXMFloat4(DirectX::XMFLOAT3(0, 1, 0), DirectX::XMConvertToDegrees(rad)));
			}

			pmdContrl->SetPosition(pmdPos);
			//pmdContrl->Draw();

			primitiveCtrl->Draw();
		}

		camera = nullptr;
		dirLight = nullptr;
		pmdContrl = nullptr;
		loader = nullptr;
	}

	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
}