#include "NormalMapCube.h"
#include "Dx12MSLib.h"
#include "BulletInclude.h"


NormalMapCube::NormalMapCube(float length, const std::string& normalMapPath) :
	mCalculator(std::make_shared<Transform3DCalculator>()),
	mCubeCtrl(PrimitiveCreator::Instance().CreateCubeNormalMap(length, normalMapPath)),
	mRotaDeg(1.0f), mNormalMap(normalMapPath), mLength(length)
{
	mInstanceMatrix.push_back(ConvertXMMATRIXToXMFloat4x4(DirectX::XMMatrixIdentity()));
	mRigidBody.push_back(PhysicsSystem::Instance().CreateRigidBody(BulletShapeType::BOX, { length,length,length }));
	mRigidBody[0]->SetCollisionState(BulletCollisionState::CHARACTER);
	mRigidBody[0]->Translate(0, 10, 0);
}

NormalMapCube::~NormalMapCube()
{
}



void NormalMapCube::AsyncRigidBody()
{
	for (int i = 0; i < mRigidBody.size(); ++i)
	{
		auto mat = mRigidBody[i]->GetWorldTransform();
		mInstanceMatrix[i] = mat;
	}
	mCubeCtrl->SetInstancingMatrix(mInstanceMatrix, 0, static_cast<unsigned int>(mInstanceMatrix.size() - 1));
}

void NormalMapCube::Roatation(DxInput & input)
{
	if (input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_NUMPAD4))
	{
		mCalculator->AddRotaY(mRotaDeg);
	}

	if (input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_NUMPAD8))
	{
		mCalculator->AddRotaX(mRotaDeg);
	}

	if (input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_NUMPAD6))
	{
		mCalculator->AddRotaY(-mRotaDeg);
	}

	if (input.IsKeyDown(VIRTUAL_KEY_INDEX::KEY_NUMPAD2))
	{
		mCalculator->AddRotaX(-mRotaDeg);
	}

	mInstanceMatrix[0] = mCalculator->GetAMatrix();
	mCubeCtrl->SetInstancingMatrix(mInstanceMatrix, 0, 0);
}

void NormalMapCube::Translate(const DirectX::XMFLOAT3 & pos, int index)
{
	mRigidBody[index]->Translate(pos);
}

void NormalMapCube::Translate(float x, float y, float z, int index)
{
	mRigidBody[index]->Translate(x, y, z);
}

void NormalMapCube::Instanceing(const DirectX::XMFLOAT3 & pos, float length)
{
	if (length == -1) length = mLength;
	Create(length);
	mRigidBody.back()->Translate(pos);
}

void NormalMapCube::Draw()
{
	mCubeCtrl->Draw();
}

void NormalMapCube::Create(float length)
{
	mInstanceMatrix.push_back(ConvertXMMATRIXToXMFloat4x4(DirectX::XMMatrixIdentity()));
	mRigidBody.push_back(PhysicsSystem::Instance().CreateRigidBody(BulletShapeType::BOX, { length,length,length }));
	mRigidBody.back()->SetCollisionState(BulletCollisionState::CHARACTER);
	std::vector<DirectX::XMFLOAT3> dummyPos(mInstanceMatrix.size());
	mCubeCtrl->Instancing(dummyPos);
	mCubeCtrl->SetColor({ 1,1,1,0 }, static_cast<int>(mInstanceMatrix.size() - 1));
}
