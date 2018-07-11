#pragma once
#include <map>
#include <string>
#include <memory>

class VMDMotion;
class File;

class VMDLoader
{
public:
	VMDLoader();
	~VMDLoader();

	std::shared_ptr<VMDMotion> LoadMotion(std::string path);
private:
	File* mFp;
	std::map<std::string, std::shared_ptr<VMDMotion>> mMotions;
	std::shared_ptr<VMDMotion> mLoadingMotion;

	void LoadHeader();
	void LoadMotionDatas();
	void CreatePoses();
};

