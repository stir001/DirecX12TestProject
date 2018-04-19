#pragma once
#include <vector>
#include <d3dcommon.h>
#include <DirectXMath.h>
class PrimitiveObject;
struct ID3D12PipelineState;

class PrimitiveManager
{
private:
	std::vector<PrimitiveObject*> objects;
public:
	void CreatePlane(DirectX::XMFLOAT3 pos,float length,float height,DirectX::XMFLOAT3 normal);
	void Draw();
	PrimitiveManager();
	~PrimitiveManager();
};

