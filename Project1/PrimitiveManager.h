#pragma once
#include <vector>
#include <d3dcommon.h>
#include <DirectXMath.h>
class PrimitiveObject;
class LightObject;

struct ID3D12PipelineState;

class PrimitiveManager
{
private:
	std::vector<PrimitiveObject*> objects;
	LightObject* light;
public:
	void SetLightObject(LightObject* inlight);
	void CreatePlane(DirectX::XMFLOAT3 pos,float length,float height,DirectX::XMFLOAT3 normal);
	void Draw();
	PrimitiveManager();
	~PrimitiveManager();
};

