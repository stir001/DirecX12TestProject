#include "File.h"
#include <iostream>


File::File(std::string path):mFp(nullptr)
{
	SetFile(path);
}


File::~File()
{
	Close();
}

void File::SetFile(const std::string& path)
{
	if (mFp != nullptr) Close();
	fopen_s(&mFp, path.data(), "rb");
}

void File::SeekFile(int seeksize)
{
	fseek(mFp, seeksize, SEEK_CUR);
}

void File::Close()
{
	if (mFp == nullptr) return;
	fclose(mFp);
	mFp = nullptr;
}