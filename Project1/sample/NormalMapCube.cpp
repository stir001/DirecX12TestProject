#include "NormalMapCube.h"
#include "PrimitiveCreator.h"
#include "PrimitiveController.h"
#include "DxInput.h"
#include "Transform3DCalculator.h"
#include "XMFloatOperators.h"


NormalMapCube::NormalMapCube(float length, const std::string& normalMapPath) :
	mCalculator(std::make_shared<Transform3DCalculator>()),
	mCubeCtrl(PrimitiveCreator::Instance().CreateCubeNormalMap(length, normalMapPath)),
	mRotaDeg(1.0f)
{
	mInstanceMatrix.push_back(StoreMatrixToXMFloat4(DirectX::XMMatrixIdentity()));
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
