#include "ModelLoader.h"
#include <wpframework.h>

ModelLoader::ModelLoader()
{
	setlocale(LC_ALL, "japanese");
}

ModelLoader::~ModelLoader()
{
}

void ModelLoader::GetRelativePath(std::string& path)
{
	relativePath.resize(path.size());
	size_t num;
	mbstowcs_s(&num, &relativePath[0], relativePath.size(), &path[0], relativePath.size() - 1);
	while (relativePath.size())
	{
		if (relativePath.back() == '/' || relativePath.back() == '\\') break;
		relativePath.pop_back();
	}
}