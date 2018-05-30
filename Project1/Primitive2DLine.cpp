#include "Primitive2DLine.h"
#include "XMFloat3Operators.h"
#include "VertexBufferObject.h"
#include "Dx12Ctrl.h"


Primitive2DLine::Primitive2DLine(const DirectX::XMFLOAT3& point1, const DirectX::XMFLOAT3& point2) :IPrimitive2D(4), mColor(0.f, 0.f, 0.f), mBaseRad(0.f), mAddRad(0.f), mScale(1.0f),mCenter(0.f,0.f,0.f)
{
	DX12CTRL_INSTANCE
	
	mVertices.resize(4);
	DirectX::XMFLOAT3 defaultColor = { 0.f ,1.f, 0.f };

	DirectX::XMFLOAT3 sub = point1 - point2;
	sub.z = 0;
	mCenter = sub * 0.5f;

	DirectX::XMFLOAT3 unitX = { 1.f, 0.f, 0.f };
	mBaseRad = acosf(DotXMFloat3(unitX, NormalizeXMFloat3(sub)));

	mLength = sqrt(DotXMFloat3(sub, sub));

	SetPoints(point1, point2);
	SetColor(defaultColor);

	UpdateBuffer();
}


Primitive2DLine::~Primitive2DLine()
{
}

void Primitive2DLine::SetCenter(const DirectX::XMFLOAT3& pos)
{
	DirectX::XMFLOAT3 unitP1 = NormalizeXMFloat3(mVertices[0].pos - mCenter);
	DirectX::XMFLOAT3 unitP2 = NormalizeXMFloat3(mVertices[3].pos - mCenter);
	mCenter = pos;

	SetPoints(unitP1 * (mLength / 2.f), unitP2 * (mLength / 2.0f));
}

void Primitive2DLine::Draw() const
{
	DX12CTRL_INSTANCE
	d12->GetCmdList()->SetPipelineState(d12->GetPiplineState(pso_primitive2D));
	d12->GetCmdList()->SetGraphicsRootSignature(d12->GetRootSignature(rsi_prm2D));
	d12->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	mVertexBuffer->SetBuffer();
	d12->GetCmdList()->DrawInstanced(4, 1, 0, 0);
}

void Primitive2DLine::UpdateBuffer()
{
	mVertexBuffer->WriteBuffer(&mVertices[0], sizeof(mVertices[0]) * mVertices.size());
}

void Primitive2DLine::SetScale(float scale)
{
	mScale = scale;

	DirectX::XMFLOAT3 point1vec = mVertices[0].pos - mCenter;
	DirectX::XMFLOAT3 point2vec = mVertices[3].pos - mCenter;

	float p1Length = GetLengthXMFloat3(point1vec);
	float p2Length = GetLengthXMFloat3(point2vec);

	float p1scale = p1Length / ((mLength * mScale) / 2);
	float p2scale = p2Length / ((mLength * mScale) / 2);

	SetPoints(NormalizeXMFloat3(point1vec) * p1scale + mCenter, NormalizeXMFloat3(point2vec) * p2scale + mCenter);
}

void Primitive2DLine::UpdateVertex()
{

}

void Primitive2DLine::SetBasePoints(const DirectX::XMFLOAT3& point1, const DirectX::XMFLOAT3& point2)
{
	mScale = 1.0f;
	mLength = GetLengthXMFloat3(point1 - point2);
	SetPoints(point1, point2);
}

void Primitive2DLine::SetPoints(const DirectX::XMFLOAT3& point1, const DirectX::XMFLOAT3& point2)
{
	DX12CTRL_INSTANCE
	DirectX::XMFLOAT2 wndSize = d12->GetWindowSize();

	mVertices[0].pos.x = point1.x  / (wndSize.x / 2.0f);
	mVertices[0].pos.y = point1.y  / (wndSize.y / 2.0f);
	mVertices[0].pos.z = point1.z;


	mVertices[1].pos.x = point2.x / (wndSize.x / 2.0f);
	mVertices[1].pos.y = point2.y / (wndSize.y / 2.0f);
	mVertices[1].pos.z = point2.z;


	mVertices[2].pos.x = (point1.x + 1.f) / (wndSize.x / 2.0f);
	mVertices[2].pos.y = (point1.y + 1.f) / (wndSize.y / 2.0f);
	mVertices[2].pos.z = point1.z;


	mVertices[3].pos.x = (point2.x + 1.f) / (wndSize.x / 2.0f);
	mVertices[3].pos.y = (point2.y + 1.f) / (wndSize.y / 2.0f);
	mVertices[3].pos.z = point2.z;
}

void Primitive2DLine::SetColor(const DirectX::XMFLOAT3& color)
{
	for (auto& v : mVertices)
	{
		v.color = color;
	}

	UpdateBuffer();
}