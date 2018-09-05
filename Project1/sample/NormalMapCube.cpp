#include "NormalMapCube.h"
#include "PrimitiveCreator.h"
#include "PrimitiveController.h"
#include "DxInput.h"
#include "Transform3DCalculator.h"


NormalMapCube::NormalMapCube(float length, std::string& normalMapPath):
	mCalculator(std::make_shared<Transform3DCalculator>()),
	mCubeCtrl(PrimitiveCreator::Instance().CreateCubeNormalMap(length, normalMapPath))
{
}


NormalMapCube::~NormalMapCube()
{
}

void NormalMapCube::Roatation(DxInput & input)
{
	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD4))
	{
		mCalculator->AddRotaY(-mRotaDeg);
	}

	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD8))
	{
		mCalculator->AddRotaX(-mRotaDeg);
	}

	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD6))
	{
		mCalculator->AddRotaY(mRotaDeg);
	}

	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_NUMPAD2))
	{
		mCalculator->AddRotaX(mRotaDeg);
	}
}

void NormalMapCube::Draw()
{
	mCubeCtrl->Draw();
}
