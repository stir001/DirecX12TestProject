#include "Primitive2DLine.h"
#include "XMFloat3Operators.h"
#include "VertexBufferObject.h"


Primitive2DLine::Primitive2DLine(const DirectX::XMFLOAT3& point1, const DirectX::XMFLOAT3& point2) :IPrimitive2D(4), mColor(0, 0, 0), mRota(0.0f)
{
	mVertices.resize(4);
	DirectX::XMFLOAT3 defaultColor = { 0.0f ,1.0f, 0.0f };

	DirectX::XMFLOAT3 sub = point1 - point2;
	sub.z = 0;

	mLength = sqrt(DotXMFloat3(sub, sub));

	mVertices[0].pos.x = point1.x;
	mVertices[0].pos.y = point1.y;
	mVertices[0].pos.z = point1.z;
	mVertices[0].color = defaultColor;

	mVertices[1].pos.x = point2.x;
	mVertices[1].pos.y = point2.y;
	mVertices[1].pos.z = point2.z;
	mVertices[1].color = defaultColor;

	mVertices[2].pos.x = point1.x + 1;
	mVertices[2].pos.y = point1.y + 1;
	mVertices[2].pos.z = point1.z;
	mVertices[2].color = defaultColor;

	mVertices[3].pos.x = point2.x + 1;
	mVertices[3].pos.y = point2.y + 1;
	mVertices[3].pos.z = point2.z;
	mVertices[3].color = defaultColor;

	UpdateBuffer();
}


Primitive2DLine::~Primitive2DLine()
{
}

void Primitive2DLine::Draw()
{

}

void Primitive2DLine::UpdateBuffer()
{
	mVertexBuffer->WriteBuffer(&mVertices[0], sizeof(mVertices[0]) * mVertices.size());
}

void Primitive2DLine::SetLength(float length)
{

}