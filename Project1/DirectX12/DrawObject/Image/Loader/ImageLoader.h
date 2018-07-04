#pragma once
#include "ShaderDatasStructure.h"
#include "DrawObjectLoader.h"

#include <string>
#include <map>
#include <memory>
#include <d3d12.h>

class ImageController;
class Image3DController;
class ImageObject;
class RootSignatureObject;
class PipelineStateObject;

class ImageLoader : public DrawObjectLoader
{
public:
	~ImageLoader();
	std::shared_ptr<ImageController> LoadImageData(const std::string& path);
	std::shared_ptr<Image3DController> LoadImage3D(const std::string& path);
	
	void Release(const std::string& releaseImagePath);

	static  std::unique_ptr<ImageLoader>& Instance()
	{
		if (mInstance == nullptr)
		{
			mInstance.reset(new ImageLoader());
		}
		return mInstance;
	}

	static void Destroy()
	{
		mInstance.reset();
	}
private:
	ImageLoader();
	ImageLoader(const ImageLoader&);
	ImageLoader(const ImageLoader&&);
	ImageLoader& operator=(const ImageLoader&);

	static std::unique_ptr<ImageLoader> mInstance;
	std::map<std::string, std::shared_ptr<ImageObject>> mImages;
	std::shared_ptr<RootSignatureObject> mRootsignature;
	std::shared_ptr<PipelineStateObject> mPipelinestate;
	std::shared_ptr<RootSignatureObject> m3DRootsignature;
	std::shared_ptr<PipelineStateObject> m3DPipelinestate;
	ShaderDatas mShader;
	ShaderDatas m3DShader;

	void CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	void CreateRootsignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev);
};

