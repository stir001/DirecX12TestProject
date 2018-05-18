#pragma once

#include <string>

class File;

class ModelLoader
{
private:
protected:
	File* fp;
	std::wstring relativePath;//‘Š‘ÎƒpƒX•Û‘¶—p
	void GetRelativePath(const std::string& path);
public:
	//virtual void Load(std::string path) = 0;
	ModelLoader();
	~ModelLoader();
};

