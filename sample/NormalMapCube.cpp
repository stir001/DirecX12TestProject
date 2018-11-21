#include "NormalMapCube.h"
#include "Dx12MSLib.h"


NormalMapCube::NormalMapCube(float length, const std::string& normalMapPath) :
	mCalculator(std::make_shared<Transform3DCalculator>()),
	mCubeCtrl(PrimitiveCreator::Instance().CreateCubeNormalMap(length, normalMapPath)),
	mRotaDeg(1.0f)
{
	mInstanceMatrix.push_back(ConvertXMMATRIXToXMFloat4x4(DirectX::XMMatrixIdentity()));
}

NormalMapCube::~NormalMapCube()
{
}

void NormalMapCube::Roatation(DxInput & input)
{
	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD4))
	{
		mCalculator->AddRotaY(mRotaDeg);
	}

	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD8))
	{
		mCalculator->AddRotaX(mRotaDeg);
	}

	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD6))
	{
		mCalculator->AddRotaY(-mRotaDeg);
	}

	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD2))
	{
		mCalculator->AddRotaX(-mRotaDeg);
	}

	mInstanceMatrix[0] = mCalculator->GetAMatrix();
	mCubeCtrl->SetInstancingMatrix(mInstanceMatrix, 0, 0);
}

void NormalMapCube::Draw()
{
	mCubeCtrl->Draw();
}
