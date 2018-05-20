#include "DirectionalLight.h"
#include "XMFloat3Operators.h"
#include "ConstantBufferObject.h"
#include "RootSignatureObject.h"
#include "Dx12Ctrl.h"

using namespace DirectX;

DirectionalLight::DirectionalLight(DirectX::XMFLOAT3 & pos, DirectX::XMFLOAT3 & dir, float length)
{
	this->pos = pos;
	this->length = length;
	element.dir = NormalizeXMFloat3(dir);

	XMVECTOR upper = { 0,1,0 };
	XMVECTOR target = { element.dir.x * length, element.dir.y * length, element.dir.z * length };

	XMVECTOR vLVec = DirectX::XMLoadFloat3(&element.dir);
	XMStoreFloat3(&element.dir, DirectX::XMVector3Normalize(vLVec));
	DirectX::XMVECTOR lightpos = XMLoadFloat3(&pos);
	DirectX::XMMATRIX lightview = DirectX::XMMatrixLookAtLH(lightpos, target, upper);
	DirectX::XMMATRIX lightprojection = DirectX::XMMatrixOrthographicLH(50, 50, 1.0f, 200.0f);//lightprojections—ñ
	element.viewProj = lightview * lightprojection;
	cbuffer = new ConstantBufferObject(sizeof(DirectionalLightElement), 1);

	cbuffer->WriteBuffer(&element, sizeof(element));
	DX12CTRL_INSTANCE
	cbuffer->SetCommandList(d12->GetCmdList().Get());
	cbuffer->SetRootparameterIndex(rpt_light);
}

DirectionalLight::DirectionalLight(float dirX, float dirY, float dirZ, float length)
{
	pos = {0, 0, 0};
	this->length = length;
	DirectX::XMFLOAT3 tdir = { dirX, dirY, dirZ };
	element.dir = NormalizeXMFloat3(tdir);

	XMVECTOR upper = { 0,1,0 };
	XMVECTOR target = { element.dir.x * length, element.dir.y * length, element.dir.z * length };

	XMVECTOR vLVec = DirectX::XMLoadFloat3(&element.dir);
	XMStoreFloat3(&element.dir, DirectX::XMVector3Normalize(vLVec));
	DirectX::XMVECTOR lightpos = XMLoadFloat3(&pos);
	DirectX::XMMATRIX lightview = DirectX::XMMatrixLookAtLH(lightpos, target, upper);
	DirectX::XMMATRIX lightprojection = DirectX::XMMatrixOrthographicLH(50, 50, 1.0f, 200.0f);//lightprojections—ñ
	element.viewProj = lightview * lightprojection;
	cbuffer = new ConstantBufferObject(sizeof(DirectionalLightElement), 1);

	cbuffer->WriteBuffer(&element, sizeof(element));
	DX12CTRL_INSTANCE
	cbuffer->SetCommandList(d12->GetCmdList().Get());
	cbuffer->SetRootparameterIndex(rpt_light);
}

DirectionalLight::~DirectionalLight()
{
	delete(cbuffer);
}

void DirectionalLight::SetLight()
{
	cbuffer->SetDescHeap();
	cbuffer->SetBuffer();
}
