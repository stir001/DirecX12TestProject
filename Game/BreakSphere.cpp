#include "BreakSphere.h"
#include "PrimitiveCutter.h"
#include <random>

BreakSphere::BreakSphere() : PrimitiveObject("BreakSphere")
{
	const float radius = 10.0f;
	const unsigned int div = 20U;
	const float height = 20.0f;

	std::random_device device;
	std::mt19937 gen(device());
	std::uniform_real_distribution<float> rand(-1.0f, 1.0f);

	DirectX::XMFLOAT3 normal(rand(gen),rand(gen),rand(gen));
	//normal = { 0.0f, 0.0f, 1.0f };
	normal = NormalizeXMFloat3(normal);

	DirectX::XMFLOAT3 origin(rand(gen) * radius * 0.8f, rand(gen)* radius * 0.8f, rand(gen)* radius * 0.8f);
	//DirectX::XMFLOAT3 origin(0.0f,0.0f,0.0f);

	//auto primitive = PrimitiveCreator::Instance().GetSphere(radius, div);
	//auto primitive = PrimitiveCreator::Instance().GetCube(radius);
	//auto primitive = PrimitiveCreator::Instance().GetCone(radius, height, div);
	auto primitive = PrimitiveCreator::Instance().GetCapsule(radius, height, div);
	//auto primitive = PrimitiveCreator::Instance().GetPlane(height, height, { 0.0f,1.0f,0.0f });
	PrimitiveCutter cutter;
	PrimitiveCutter::CutData data;
	data.primitive.verts = primitive->GetVertices();
	data.primitive.indices = primitive->GetIndices();
	data.face.normal = normal;
	data.face.origin = origin;
	auto verts = cutter.CutPrimitive(data);
	mVertices = verts.minus.verts;
	mIndices = verts.minus.indices;
	/*mVertices = verts.plus.verts;
	mIndices = verts.plus.indices;*/
}

BreakSphere::~BreakSphere()
{
}

