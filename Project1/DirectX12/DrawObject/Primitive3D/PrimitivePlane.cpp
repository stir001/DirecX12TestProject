#include "PrimitivePlane.h"
#include "Dx12Ctrl.h"
#include "d3dx12.h"
#include "XMFloatOperators.h"
#include "VertexBufferObject.h"
#include "RootSignatureObject.h"
#include "PipelineStateObject.h"

using namespace DirectX;

PrimitivePlane::PrimitivePlane()
{
}

PrimitivePlane::PrimitivePlane(DirectX::XMFLOAT3 p, float len, float hei, DirectX::XMFLOAT3 norm)
{
	mPos = p;
	mLength = len;
	mHeight = hei;
	norm = NormalizeXMFloat3(norm);

	DirectX::XMVECTOR leftUp = { -mLength / 2.0f, 0, mHeight / 2.0f };
	DirectX::XMVECTOR rightUp = { mLength / 2.0f, 0, mHeight / 2.0f };
	DirectX::XMVECTOR leftDown = { -mLength / 2.0f, 0, -mHeight / 2.0f };
	DirectX::XMVECTOR rightDown = { mLength / 2.0f, 0,  -mHeight / 2.0f };

	DirectX::XMVECTOR center = { p.x, p.y, p.z };

	DirectX::XMFLOAT3 defNorm = { 0, 1, 0 };

	DirectX::XMFLOAT3 inNorm = { norm.x, norm.y, norm.z };

	float cos = DotXMFloat3(defNorm, inNorm);
	float rad = acosf(DotXMFloat3(defNorm, inNorm));


	DirectX::XMMATRIX m = XMMatrixIdentity();

	if (defNorm != inNorm)
	{
		DirectX::XMFLOAT3 cross = CrossXMFloat3(defNorm, inNorm);

		DirectX::XMVECTOR q = CreateQuoternion(cross, XMConvertToDegrees(rad));

		m = XMMatrixRotationQuaternion(q);
	}

	leftUp = XMVector3Transform(leftUp, m);
	rightUp = XMVector3Transform(rightUp, m);
	leftDown = XMVector3Transform(leftDown, m);
	rightDown = XMVector3Transform(rightDown, m);

	DirectX::XMFLOAT3 luPos;
	XMStoreFloat3(&luPos, leftUp + center);

	DirectX::XMFLOAT3 ruPos;
	XMStoreFloat3(&ruPos, rightUp + center);

	DirectX::XMFLOAT3 ldPos;
	XMStoreFloat3(&ldPos, leftDown + center);

	DirectX::XMFLOAT3 rdPos;
	XMStoreFloat3(&rdPos, rightDown + center);

	unsigned int vertexCount = 4;
	mVertices.reserve(vertexCount);
	DirectX::XMFLOAT2 uv = { 0,0 };
	mVertices.push_back(PrimitiveVertex(luPos, norm, uv));

	uv.x = 1;
	mVertices.push_back(PrimitiveVertex(ruPos, norm, uv));

	uv.x = 0;
	uv.y = 1;
	mVertices.push_back(PrimitiveVertex(ldPos, norm,uv));

	uv.x = 1;
	mVertices.push_back(PrimitiveVertex(rdPos, norm, uv));

	int size = sizeof(PrimitiveVertex);

	mVertexBuffer.reset(new VertexBufferObject("PrimitiveVertexBuffer",Dx12Ctrl::Instance().GetDev(),size, vertexCount));
	mVertexBuffer->WriteBuffer(&mVertices[0], size * vertexCount);
}

PrimitivePlane::~PrimitivePlane()
{
}

void PrimitivePlane::Draw()
{
	DX12CTRL_INSTANCE
	d12.GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	mVertexBuffer->SetBuffer(d12.GetCmdList());
	d12.GetCmdList()->DrawInstanced(static_cast<UINT>(mVertices.size()), 1, 0, 0);
}
