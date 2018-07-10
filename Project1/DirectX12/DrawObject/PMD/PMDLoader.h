#pragma once
#include "DrawObjectLoader.h"
#include <map>

class PMDModel;
class PMDController;

class PMDLoader :
	public DrawObjectLoader
{
public:
	PMDLoader();
	~PMDLoader();

	std::shared_ptr<PMDController> Load(std::string path);
private:

	std::map<std::string,std::shared_ptr<PMDModel>> mModels;
	std::shared_ptr<PMDModel> mLoadingmodel;
	std::shared_ptr<PMDController> mController;
	int exittexcount;
	void LoadHeader();
	void LoadVertex();
	void LoadIndex();
	void LoadMaterial();
	void LoadBone();
	void LoadIkList();
	void LoadSkin();
	void LoadSkinDisp();
	void LoadBoneFrameDisp();
	void LoadBoneDisp();
	void SeekEngligh();
	void LoadToonPath();
	void LoadRigidBody();
	void LoadJoint();

	void CreateIndexBuffer();
	void CreateVertexBuffer();
	void CreateTexture();
	void CreateMaterialBuffer();
	void CreateBoneMatrixBuffer();

	void CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	void CreateRootsignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev);
};	

