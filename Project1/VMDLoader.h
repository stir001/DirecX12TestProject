#pragma once
#include <map>
#include <string>

class VMDMotion;
class File;

class VMDLoader
{
private:
	File* fp;
	std::map<std::string, VMDMotion*> motions;
	VMDMotion* loadingMotion;

	void LoadHeader();
	void LoadMotionDatas();
	void CreatePoses();
public:
	VMDMotion* LoadMotion(std::string path);

	VMDLoader();
	~VMDLoader();
};

