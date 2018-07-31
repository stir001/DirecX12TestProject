#include "Dx12Camera.h"
#include "ConstantBufferObject.h"
#include "Dx12Ctrl.h"
#include "XMFloatOperators.h"
#include "DxInput.h"

using namespace DirectX;

Dx12Camera::Dx12Camera(int wWidth,int wHeight):mWidth(wWidth),mHeight(wHeight)
, mElement{ {0,20,-30,1},{0,9,0,1}}
, mUpper(0, 1, 0),mLocalUpper(mUpper)
{
	XMVECTOR vEye = DirectX::XMLoadFloat4(&mElement.eye);
	XMVECTOR vTarget = DirectX::XMLoadFloat4(&mElement.target);
	XMVECTOR vUpper = DirectX::XMLoadFloat3(&mUpper);
	DirectX::XMStoreFloat4x4(&mCamera , DirectX::XMMatrixLookAtLH(vEye, vTarget, vUpper));
	DirectX::XMStoreFloat4x4(&mProjection,DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, static_cast<float>(mWidth) / static_cast<float>(mHeight), 20.0f, 500.f));//カメラのプロジェクション行列
	DirectX::XMStoreFloat4x4(&mElement.view, DirectX::XMLoadFloat4x4(&mCamera));
	DirectX::XMStoreFloat4x4(&mElement.projection, DirectX::XMLoadFloat4x4(&mProjection));
	DirectX::XMStoreFloat4x4(&mElement.world, DirectX::XMMatrixIdentity());
	mWorldRotation = mElement.world;

	mCameraBuffer.reset(new ConstantBufferObject("CameraConstantBuffer", Dx12Ctrl::Instance().GetDev(),sizeof(mElement), 1));

	UpdateBuffer();
}

Dx12Camera::Dx12Camera(int wWidth, int wHeight, DirectX::XMFLOAT3& eye, DirectX::XMFLOAT3& target, DirectX::XMFLOAT3& upper)
	:mWidth(wWidth),mHeight(wHeight)
	, mElement{}, mUpper(upper), mLocalUpper(mUpper)
{
	mElement.eye = { eye.x,eye.y,eye.z,1 };
	mElement.eye = { target.x,target.y,target.z,1 };
	XMVECTOR vEye = DirectX::XMLoadFloat4(&mElement.eye);
	XMVECTOR vTarget = DirectX::XMLoadFloat4(&mElement.target);
	XMVECTOR vUpper = DirectX::XMLoadFloat3(&upper);
	DirectX::XMStoreFloat4x4(&mProjection , DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, static_cast<float>(mWidth) / static_cast<float>(mHeight), 20.0f, 500.f));//カメラのプロジェクション行列
	DirectX::XMStoreFloat4x4(&mElement.view, DirectX::XMLoadFloat4x4(&mCamera));
	DirectX::XMStoreFloat4x4(&mElement.projection, DirectX::XMLoadFloat4x4(&mProjection));
	DirectX::XMStoreFloat4x4(&mElement.world, DirectX::XMMatrixIdentity());
	mWorldRotation = mElement.world;
	UpdateElement();

	mCameraBuffer.reset(new ConstantBufferObject("CameraConstantBuffer", Dx12Ctrl::Instance().GetDev(),sizeof(mElement), 1));

	UpdateBuffer();
}

Dx12Camera::~Dx12Camera()
{
	mCameraBuffer.reset();
}

void Dx12Camera::UpdateElement()
{
	DirectX::XMStoreFloat4x4(&mCamera, DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat4(&mElement.eye)
		, DirectX::XMLoadFloat4(&mElement.target)
		, DirectX::XMLoadFloat3(&mUpper)));
	DirectX::XMStoreFloat4x4(&mElement.view, DirectX::XMLoadFloat4x4(&mCamera));
	DirectX::XMStoreFloat4x4(&mElement.projection, DirectX::XMLoadFloat4x4(&mProjection));
	
}

void Dx12Camera::UpdateBuffer()
{	
	mCameraBuffer->WriteBuffer(&mElement, sizeof(mElement));
	size_t s = sizeof(mElement);
}

void Dx12Camera::SetPos(DirectX::XMFLOAT3& pos)
{
	XMFLOAT3 t = 
		{ mElement.target.x + pos.x - mElement.eye.x
		, mElement.target.y + pos.y - mElement.eye.y
		, mElement.target.z + pos.z - mElement.eye.z };
	mElement.eye = { pos.x,pos.y,pos.z ,1};
	mElement.target = { t.x,t.y,t.z,1 };
	UpdateElement();
	UpdateBuffer();
}

void Dx12Camera::SetTarget(DirectX::XMFLOAT3& target)
{
	mElement.target = { target.x, target.y, target.z, 1 };
	UpdateElement();
	UpdateBuffer();
}

void Dx12Camera::AddXAxisRota(float deg)
{
	AddRotationAxis(XMMatrixRotationX(XMConvertToRadians(deg)));
	UpdateElement();
	UpdateBuffer();
}

void Dx12Camera::AddYAxisRota(float deg)
{
	AddRotationAxis(XMMatrixRotationY(XMConvertToRadians(deg)));
	UpdateElement();
	UpdateBuffer();
}

void Dx12Camera::AddZAxisRota(float deg)
{
	AddRotationAxis(XMMatrixRotationZ(XMConvertToRadians(deg)));
	UpdateElement();
	UpdateBuffer();
}

void Dx12Camera::MoveUp(float vel)
{
	mElement.eye.y += vel;
	mElement.target.y += vel;
	UpdateElement();
	UpdateBuffer();
}

void Dx12Camera::MoveFront(float vel)
{
	DirectX::XMFLOAT3 target = { mElement.target.x, mElement.target.y ,mElement.target.z };
	DirectX::XMFLOAT3 eye = { mElement.eye.x, mElement.eye.y ,mElement.eye.z };
	XMFLOAT3 eyeToTarget = target - eye;
	eyeToTarget = NormalizeXMFloat3(eyeToTarget);

	eyeToTarget *= vel;
	mElement.eye = { mElement.eye.x + eyeToTarget.x,mElement.eye.y + eyeToTarget.y, mElement.eye.z + eyeToTarget.z,1 };
	mElement.target = { mElement.target.x + eyeToTarget.x,mElement.target.y + eyeToTarget.y, mElement.target.z + eyeToTarget.z, 1 };
	UpdateElement();
	UpdateBuffer();
}

void Dx12Camera::MoveSide(float vel)
{
	DirectX::XMFLOAT3 eyeToTarget = GetEyeToTargetVec();
	DirectX::XMFLOAT3 cross = CrossXMFloat3(eyeToTarget, GetLocalUpper());
	auto testdotvalue = DotXMFloat3(eyeToTarget, GetLocalUpper());
	cross = NormalizeXMFloat3(cross) * vel;
	mElement.eye += cross;
	mElement.target += cross;
	UpdateElement();
	UpdateBuffer();
}

void Dx12Camera::TurnRightLeft(float deg)
{
	DirectX::XMVECTOR t = DirectX::XMLoadFloat4(&mElement.target);
	DirectX::XMVECTOR e = DirectX::XMLoadFloat4(&mElement.eye);
	DirectX::XMFLOAT3 eyeToTarget = { mElement.target.x - mElement.eye.x, mElement.target.y - mElement.eye.y, mElement.target.z - mElement.eye.z };
	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslationFromVector(-e);
	trans *= DirectX::XMMatrixRotationQuaternion(CreateQuoternion(GetLocalUpper(), deg));
	trans *= DirectX::XMMatrixTranslationFromVector(e);
	t = DirectX::XMVector4Transform(t - e,trans);
	DirectX::XMStoreFloat4(&mElement.target, e + t);

	UpdateElement();
	UpdateBuffer();
}

void Dx12Camera::TurnUpDown(float deg)
{
	DirectX::XMFLOAT3 eyeToTarget = GetEyeToTargetVec();
	DirectX::XMFLOAT4 target = mElement.target;
	DirectX::XMFLOAT3 cross = CrossXMFloat3(eyeToTarget, GetLocalUpper());
	cross = NormalizeXMFloat3(cross);
	DirectX::XMVECTOR t = DirectX::XMLoadFloat4(&mElement.target);
	DirectX::XMVECTOR e = DirectX::XMLoadFloat4(&mElement.eye);
	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslationFromVector(-e);
	trans *= DirectX::XMMatrixRotationQuaternion(CreateQuoternion(cross, deg));
	trans *= DirectX::XMMatrixTranslationFromVector(e);
	t = DirectX::XMVector4Transform(t - e, trans);
	DirectX::XMStoreFloat4(&mElement.target, e + t);

	float dotvalue = abs(DotXMFloat3(NormalizeXMFloat3(GetEyeToTargetVec()), mUpper));
	if (dotvalue >= 0.95f)
	{
		mElement.target = target;
		return;
	}

	UpdateElement();
	UpdateBuffer();
}

void Dx12Camera::DefaultMove(const DxInput& input)
{
	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_W))
	{
		MoveFront(1.0f);
	}

	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_A))
	{
		MoveSide(1.0f);
	}

	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_D))
	{
		MoveSide(-1.0f);
	}

	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_S))
	{
		MoveFront(-1.0f);
	}

	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_SPACE))
	{
		MoveUp(1.0f);
	}

	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_LSHIFT))
	{
		MoveUp(-1.0f);
	}

	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_UP))
	{
		TurnUpDown(1.0f);
	}
	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_DOWN))
	{
		TurnUpDown(-1.0f);
	}
	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_RIGHT))
	{
		TurnRightLeft(1.0f);
	}
	if (input.IsKeyDown(eVIRTUAL_KEY_INDEX_LEFT))
	{
		TurnRightLeft(-1.0f);
	}
}

DirectX::XMFLOAT4X4 Dx12Camera::GetView()
{
	return mCamera;
}

DirectX::XMFLOAT4X4 Dx12Camera::GetProjection()
{
	return mProjection;
}

DirectX::XMFLOAT4 Dx12Camera::GetCameraPosition()
{
	return mElement.eye;
}

DirectX::XMFLOAT4X4 Dx12Camera::GetWorld()
{
	return mElement.world;
}

std::shared_ptr<ConstantBufferObject>& Dx12Camera::GetCameraBuffer()
{
	return mCameraBuffer;
}

void Dx12Camera::AddRotationAxis(DirectX::XMMATRIX& rotaMatrix)
{
	DirectX::XMStoreFloat4x4(&mWorldRotation, /*DirectX::XMLoadFloat4x4(&mWorldRotation) **/ rotaMatrix);
	DirectX::XMFLOAT3 ePos = { mElement.eye.x, mElement.eye.y, mElement.eye.z };
	DirectX::XMVECTOR eyeVec = DirectX::XMLoadFloat3(&ePos);
	DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslationFromVector(-eyeVec);
	DirectX::XMMATRIX rtransMatrix = DirectX::XMMatrixTranslationFromVector(eyeVec);

	DirectX::XMFLOAT4X4 tlansMatrix;
	DirectX::XMStoreFloat4x4(&tlansMatrix, translationMatrix * DirectX::XMLoadFloat4x4(&mWorldRotation) * rtransMatrix);
	mElement.target *= tlansMatrix;
}

DirectX::XMFLOAT3 Dx12Camera::GetEyeToTargetVec()
{
	return DirectX::XMFLOAT3(mElement.target.x - mElement.eye.x, mElement.target.y - mElement.eye.y, mElement.target.z - mElement.eye.z);
}

DirectX::XMFLOAT3 Dx12Camera::GetLocalUpper()
{
	DirectX::XMFLOAT3 eyeToTarget = GetEyeToTargetVec();
	DirectX::XMFLOAT3 crossVec = CrossXMFloat3(eyeToTarget, mUpper);
	crossVec = NormalizeXMFloat3(crossVec);
	mLocalUpper = CrossXMFloat3(crossVec, eyeToTarget);
	return  mLocalUpper;
}