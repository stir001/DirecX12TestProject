#include "DirectionalLight.h"
#include "XMFloatOperators.h"
#include "ConstantBufferObject.h"
#include "RootSignatureObject.h"
#include "Dx12Ctrl.h"

using namespace DirectX;

DirectionalLight::DirectionalLight(DirectX::XMFLOAT3 & pos, DirectX::XMFLOAT3 & dir, float length)
{
	mPos = pos;
	mLength = length;
	mElement.dir = StoreToXMFloat4(NormalizeXMFloat3(dir));

	XMVECTOR upper = { 0,1,0 };
	XMVECTOR target = { mElement.dir.x * length, mElement.dir.y * length, mElement.dir.z * length };

	DirectX::XMFLOAT3 t_dir = { mElement.dir.x, mElement.dir.y, mElement.dir.z };
	mElement.dir = StoreToXMFloat4(NormalizeXMFloat3(t_dir));
	DirectX::XMVECTOR lightpos = XMLoadFloat3(&pos);
	DirectX::XMMATRIX lightview = DirectX::XMMatrixLookAtLH(lightpos, target, upper);
	DirectX::XMMATRIX lightprojection = DirectX::XMMatrixOrthographicLH(50, 50, 1.0f, 200.0f);//lightprojections—ñ
	XMStoreFloat4x4(&mElement.viewProj, lightview * lightprojection);
	mCbuffer.reset(new ConstantBufferObject("DirectionalLightConstantBuffer",Dx12Ctrl::Instance()->GetDev(),sizeof(DirectionalLightElement), 1));

	mCbuffer->WriteBuffer(&mElement, sizeof(mElement));
}

DirectionalLight::DirectionalLight(float dirX, float dirY, float dirZ, float length)
{
	mPos = {0, 0, 0};
	mLength = length;
	DirectX::XMFLOAT3 tdir = { dirX, dirY, dirZ };
	mElement.dir = StoreToXMFloat4(NormalizeXMFloat3(tdir));

	XMVECTOR upper = { 0,1,0 };
	XMVECTOR target = { mElement.dir.x * length, mElement.dir.y * length, mElement.dir.z * length };

	DirectX::XMFLOAT3 t_dir = { mElement.dir.x, mElement.dir.y, mElement.dir.z };
	mElement.dir = StoreToXMFloat4(NormalizeXMFloat3(t_dir));
	DirectX::XMVECTOR lightpos = XMLoadFloat3(&mPos);
	DirectX::XMMATRIX lightview = DirectX::XMMatrixLookAtLH(lightpos, target, upper);
	DirectX::XMMATRIX lightprojection = DirectX::XMMatrixOrthographicLH(50, 50, 1.0f, 200.0f);//lightprojections—ñ
	XMStoreFloat4x4(&mElement.viewProj, lightview * lightprojection);
	mCbuffer.reset(new ConstantBufferObject("DirectionalLightConstantBuffer", Dx12Ctrl::Instance()->GetDev(),sizeof(DirectionalLightElement), 1));

	mCbuffer->WriteBuffer(&mElement, sizeof(mElement));
}

DirectionalLight::~DirectionalLight()
{
}

