#pragma once

#include <string>

class File;

class ModelLoader
{
private:
protected:
	File* fp;
	std::wstring relativePath;//相対パス保存用
	void GetRelativePath(std::string& path);
public:
	//virtual void Load(std::string path) = 0;
	ModelLoader();
	~ModelLoader();
};

