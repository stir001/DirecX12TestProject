#pragma once
#include "ModelLoader.h"
#include <vector>

class PMDModel;
class PMDController;

class PMDLoader :
	public ModelLoader
{
private:
	std::vector<PMDModel*> models;
	PMDModel* loadingmodel;
	PMDController* controller;
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
public:
	PMDController* Load(std::string path);
	PMDLoader();
	~PMDLoader();
};

