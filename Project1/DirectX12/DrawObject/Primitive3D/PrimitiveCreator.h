#pragma once
#include <vector>
#include <d3dcommon.h>
#include <DirectXMath.h>
#include <memory>

class PrimitiveObject;
class LightObject;
class PrimitivePlane;

class PrimitiveCreator
{
public:
	PrimitiveCreator();
	~PrimitiveCreator();

	void SetLightObject(std::shared_ptr<LightObject> inlight);
	std::shared_ptr<PrimitivePlane> CreatePlane(DirectX::XMFLOAT3 pos, float length, float height, DirectX::XMFLOAT3 normal);
private:
	std::shared_ptr<LightObject> mLight;
};

