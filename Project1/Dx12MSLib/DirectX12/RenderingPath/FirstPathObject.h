#pragma once
#include "RenderingPath/Base/RenderingPathObject.h"

struct ID3D12Device;
class DepthBufferObject;
class RendertargetObject;

class FirstPathObject :
	public RenderingPathObject
{
public:
	FirstPathObject(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, std::shared_ptr<DepthBufferObject> depthBuffer,
		unsigned int wWidth, unsigned int wHeight);
	~FirstPathObject();

	void FirstUpdate();

	void PreExcutePath();

	void ExcutePath(Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue);

	void ResetCommandList();

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList();

	Microsoft::WRL::ComPtr<ID3D12Resource> GetRenderTarget();
private:
	std::shared_ptr<DepthBufferObject> mDepthBuffer;
	std::shared_ptr<RendertargetObject> mRendertarget;
};

