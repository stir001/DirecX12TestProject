#pragma once
#include <vector>
#include <d3d12.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <memory>

class LightObject;
class PrimitivePlane;
class RootSignatureObject;
class PipelineStateObject;

class PrimitiveCreator
{
public:
	PrimitiveCreator();
	~PrimitiveCreator();

	void SetLightObject(std::shared_ptr<LightObject> inlight);
	std::shared_ptr<PrimitivePlane> CreatePlane(DirectX::XMFLOAT3 pos, float length, float height, DirectX::XMFLOAT3 normal);
private:
	std::shared_ptr<LightObject> mLight;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommnadList;
	std::shared_ptr<RootSignatureObject> mRootsiganture;
	std::shared_ptr<PipelineStateObject> mPipelineState;
};

