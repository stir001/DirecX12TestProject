#pragma once
#include <vector>
#include <d3dcommon.h>
#include <DirectXMath.h>
#include <memory>

class PrimitiveObject;
class LightObject;

struct ID3D12PipelineState;

class PrimitiveManager
{
private:
	std::vector<PrimitiveObject*> objects;
	std::shared_ptr<LightObject> light;
public:
	void SetLightObject(std::shared_ptr<LightObject> inlight);
	void CreatePlane(DirectX::XMFLOAT3 pos,float length,float height,DirectX::XMFLOAT3 normal);
	void Draw();
	PrimitiveManager();
	~PrimitiveManager();
};

