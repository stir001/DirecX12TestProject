#include "VMDLoader.h"
#include "VMDMotion.h"
#include "File.h"

#include <algorithm>

VMDLoader::VMDLoader()
{
}


VMDLoader::~VMDLoader()
{
	fp->Close();
	delete fp;
	for (auto m : motions) delete m.second;
	loadingMotion = nullptr;
}

VMDMotion* VMDLoader::LoadMotion(std::string path)
{
	if (motions.find(path) != motions.end()) return motions[path];
	fp = new File(path);
	loadingMotion = new VMDMotion();

	LoadHeader();
	LoadMotionDatas();

	fp->Close();

	CreatePoses();
	motions[path] = loadingMotion;
	return loadingMotion;
}

void VMDLoader::LoadHeader()
{
	fp->LoadFile(&loadingMotion->header.header[0], sizeof(loadingMotion->header.header));
	fp->LoadFile(&loadingMotion->header.modelName[0], sizeof(loadingMotion->header.modelName));
	fp->LoadFile(&loadingMotion->header.count);
}

void VMDLoader::LoadMotionDatas()
{
	loadingMotion->motions.resize(loadingMotion->header.count);
	for (unsigned int i = 0; i < loadingMotion->header.count; i++)
	{
		fp->LoadFile(&loadingMotion->motions[i].boneName[0],sizeof(loadingMotion->motions[i].boneName));
		fp->LoadFile(&loadingMotion->motions[i].frameNo);
		fp->LoadFile(&loadingMotion->motions[i].location);
		fp->LoadFile(&loadingMotion->motions[i].quoternion);
		fp->LoadFile(&loadingMotion->motions[i].interpolation[0],sizeof(loadingMotion->motions[i].interpolation));
	}
}

void VMDLoader::CreatePoses()
{
	for (auto& m : loadingMotion->motions)
	{
		std::string boneName = m.boneName;
		VMDPose p;
		p.frameNo = m.frameNo;
		p.quoternion = m.quoternion;//DirectX::XMLoadFloat4(&m.quoternion);
		loadingMotion->poses[boneName].push_back(p);
	}

	for (auto& p : loadingMotion->poses)
	{
		std::sort(p.second.begin(), p.second.end(), [&](const VMDPose& lval, const VMDPose& rval) {return lval.frameNo < rval.frameNo; });
	}
}