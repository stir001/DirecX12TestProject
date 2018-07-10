#include "Dx12Camera.h"
#include "ConstantBufferObject.h"
#include "Dx12Ctrl.h"
#include "RootSignatureObject.h"
#include "XMFloat3Operators.h"

using namespace DirectX;

Dx12Camera::Dx12Camera(int wWidth,int wHeight):width(wWidth),height(wHeight)
, element{ {0,20,-30},{0,9,0},DirectX::XMMatrixIdentity(),DirectX::XMMatrixIdentity() }
, upper(0,1,0)
{
	XMVECTOR vEye = DirectX::XMLoadFloat3(&element.eye);
	XMVECTOR vTarget = DirectX::XMLoadFloat3(&element.target);
	XMVECTOR vUpper = DirectX::XMLoadFloat3(&upper);
	camera = DirectX::XMMatrixLookAtLH(vEye, vTarget, vUpper);
	projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, static_cast<float>(width) / static_cast<float>(height), 1.0f, 500.f);//カメラのプロジェクション行列
	element.viewproj = camera * projection;

	cameraBuffer = new ConstantBufferObject(sizeof(element), 1);
	DX12CTRL_INSTANCE
	cameraBuffer->SetCommandList(d12.GetCmdList().Get());
	cameraBuffer->SetRootparameterIndex(1);

	UpdateBuffer();
}

Dx12Camera::Dx12Camera(int wWidth, int wHeight, DirectX::XMFLOAT3& eye, DirectX::XMFLOAT3& target, DirectX::XMFLOAT3& upper)
	:width(wWidth),height(wHeight)
	, element{ eye,target,DirectX::XMMatrixIdentity(),DirectX::XMMatrixIdentity() }
	, upper(upper)
{
	XMVECTOR vEye = DirectX::XMLoadFloat3(&element.eye);
	XMVECTOR vTarget = DirectX::XMLoadFloat3(&element.target);
	XMVECTOR vUpper = DirectX::XMLoadFloat3(&upper);
	projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, static_cast<float>(width) / static_cast<float>(height), 10.0f, 500.f);//カメラのプロジェクション行列
	UpdateElement();

	DX12CTRL_INSTANCE
	cameraBuffer->SetCommandList(d12.GetCmdList().Get());
	cameraBuffer = new ConstantBufferObject(sizeof(element), 1);

	UpdateBuffer();
}

Dx12Camera::~Dx12Camera()
{
}

void Dx12Camera::UpdateElement()
{
	camera = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&element.eye)
		, DirectX::XMLoadFloat3(&element.target)
		, DirectX::XMLoadFloat3(&upper));
	element.viewproj = camera * projection;
}

void Dx12Camera::UpdateBuffer()
{	
	cameraBuffer->WriteBuffer(&element, sizeof(element));
}

void Dx12Camera::SetBuffer()
{
	cameraBuffer->SetDescHeap();
	cameraBuffer->SetBuffer();
}

void Dx12Camera::SetPos(DirectX::XMFLOAT3& pos)
{
	XMFLOAT3 t = 
		{ element.target.x + pos.x - element.eye.x
		, element.target.y + pos.y - element.eye.y
		, element.target.z + pos.z - element.eye.z };
	element.eye = pos;
	element.target = t;
	UpdateElement();
	UpdateBuffer();
}

void Dx12Camera::AddXAxisRota(float deg)
{
	element.world = XMMatrixRotationX(XMConvertToRadians(deg)) * element.world;
	UpdateBuffer();
}

void Dx12Camera::AddYAxisRota(float deg)
{
	element.world = XMMatrixRotationY(XMConvertToRadians(deg)) * element.world;
	UpdateBuffer();
}

void Dx12Camera::AddZAxisRota(float deg)
{
	element.world = XMMatrixRotationZ(XMConvertToRadians(deg)) * element.world;
	UpdateBuffer();
}

void Dx12Camera::MoveUp(float vel)
{
	element.eye.y += vel;
	element.target.y += vel;
	UpdateElement();
	UpdateBuffer();
}

void Dx12Camera::MoveFront(float vel)
{
	XMFLOAT3 eyeToTarget = element.target - element.eye;
	eyeToTarget = NormalizeXMFloat3(eyeToTarget);

	eyeToTarget *= vel;
	element.eye += eyeToTarget;
	element.target += eyeToTarget;
	UpdateElement();
	UpdateBuffer();
}

void Dx12Camera::MoveSide(float vel)
{
	XMFLOAT3 eyeToTarget = element.target - element.eye;
	XMFLOAT3 cross = CrossXMFloat3(eyeToTarget, upper);
	cross = NormalizeXMFloat3(cross) * vel;
	element.eye += cross;
	element.target += cross;
	UpdateElement();
	UpdateBuffer();
}

void Dx12Camera::TurnRightLeft(float deg)
{
	DirectX::XMVECTOR t = DirectX::XMLoadFloat3(&element.target);
	DirectX::XMVECTOR e = DirectX::XMLoadFloat3(&element.eye);
	XMFLOAT3 eyeToTarget = element.target - element.eye;
	XMFLOAT3 cross = CrossXMFloat3(eyeToTarget, upper);
	XMMATRIX trans = DirectX::XMMatrixTranslationFromVector(-e);
	cross = CrossXMFloat3(eyeToTarget, cross);
	trans *= DirectX::XMMatrixRotationQuaternion(CreateQuoternion(cross, deg));
	trans *= DirectX::XMMatrixTranslationFromVector(e);
	t = DirectX::XMVector4Transform(t - e,trans);
	DirectX::XMStoreFloat3(&element.target, e + t);
	UpdateElement();
	UpdateBuffer();
}

void Dx12Camera::TurnUpDown(float deg)
{
	XMFLOAT3 eyeToTarget = element.target - element.eye;
	XMFLOAT3 cross = CrossXMFloat3(eyeToTarget, upper);
	cross = NormalizeXMFloat3(cross);
	DirectX::XMVECTOR t = DirectX::XMLoadFloat3(&element.target);
	DirectX::XMVECTOR e = DirectX::XMLoadFloat3(&element.eye);
	XMMATRIX trans = DirectX::XMMatrixTranslationFromVector(-e);
	trans *= DirectX::XMMatrixRotationQuaternion(CreateQuoternion(cross, deg));
	trans *= DirectX::XMMatrixTranslationFromVector(e);
	t = DirectX::XMVector4Transform(t - e, trans);
	DirectX::XMStoreFloat3(&element.target, e + t);

	UpdateElement();
	UpdateBuffer();
}