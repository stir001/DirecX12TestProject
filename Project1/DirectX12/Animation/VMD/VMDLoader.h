#pragma once
#include <map>
#include <string>

class VMDMotion;
class File;

class VMDLoader
{
public:
	VMDLoader();
	~VMDLoader();

	VMDMotion* LoadMotion(std::string path);
private:
	File* mFp;
	std::map<std::string, VMDMotion*> mMotions;
	VMDMotion* mLoadingMotion;

	void LoadHeader();
	void LoadMotionDatas();
	void CreatePoses();
};

