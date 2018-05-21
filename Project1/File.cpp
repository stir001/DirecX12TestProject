#include "File.h"
#include <iostream>
#include <cassert>


File::File(std::string path):fp(nullptr)
{
	SetFile(path);
}


File::~File()
{
}

void File::SetFile(std::string path)
{
	if (fp != nullptr)
	{
		Close();
	}
	fp = nullptr;
	fopen_s(&fp, path.data(), "rb");
}

void File::SeekFile(int seeksize)
{
	fseek(fp, seeksize, SEEK_CUR);
}

void File::Close()
{
	if (fp == nullptr) return;
	fclose(fp);
	fp = nullptr;
}