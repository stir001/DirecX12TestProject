#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <memory>
#include <string>

class TextureObject;
class ShaderResourceObject;

struct ID3D12GraphicsCommandList;
struct ImageVertex;

class ImageObject
{
	friend class ImageLoader;
public:
	~ImageObject();
	DirectX::XMFLOAT2 GetImageSize();

	std::shared_ptr<ShaderResourceObject> GetShaderResource() const;
	const std::string& GetTextureName() const;
	const std::string& GetFilePath() const;
private:
	enum eROOT_PARAMATER_INDEX
	{
		eROOT_PARAMATER_INDEX_TEXTURE
	};

	int mWidth;
	int mHeight;
	std::shared_ptr<TextureObject> mTexObj;

	ImageObject(int width, int height, std::shared_ptr<TextureObject> texObj);
};

