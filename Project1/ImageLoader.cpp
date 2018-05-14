#include "ImageLoader.h"
#include "ImageObject.h"
#include "Dx12Ctrl.h"
#include "CharToWChar.h"
#include "TextureObj.h"
#include "ImageController.h"

ImageLoader::ImageLoader()
{
}


ImageLoader::~ImageLoader()
{
}

std::shared_ptr<ImageController> ImageLoader::LoadImageData(const std::string& path)
{
	std::shared_ptr<ImageController> imgCtrl;
	auto itr = images.find(path);
	if (itr != images.end())
	{
		imgCtrl.reset(new ImageController(itr->second));
		return imgCtrl;
	}
	DX12CTRL_INSTANCE
	const char* cPath = path.data();
	size_t size = path.size() + 1;
	wchar_t* wchar = new wchar_t[size];
	size_t convert = ToWChar(&wchar, size, cPath, size);
	std::wstring wpath = wchar;
	ID3D12DescriptorHeap* t_texDescHeap = d12->CreateTextureDescHeap(path, 1);
	TextureObj* tObj = d12->LoadTexture(wpath, t_texDescHeap->GetCPUDescriptorHandleForHeapStart(), t_texDescHeap->GetGPUDescriptorHandleForHeapStart());
	ImageObject* imgObj = new ImageObject(tObj->GetWidth(), tObj->GetHeight(), tObj, t_texDescHeap);
	std::shared_ptr<ImageObject> sp;
	sp.reset(imgObj);
	images[path] = sp;
	imgCtrl.reset(new ImageController(sp));
	return imgCtrl;
}
