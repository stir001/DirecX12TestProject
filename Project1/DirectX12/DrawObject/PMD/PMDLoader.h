#pragma once
#include "DrawObjectLoader.h"
#include <vector>

class PMDModel;
class PMDController;

class PMDLoader :
	public DrawObjectLoader
{
public:
	PMDLoader();
	~PMDLoader();

	PMDController* Load(std::string path);
private:

	std::vector<PMDModel*> mModels;
	PMDModel* mLoadingmodel;
	PMDController* mController;
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

	void CreatePiplineState();
	void CreateRootSiganature();
};

