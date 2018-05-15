#pragma once
#include <string>
#include <iostream>

//typedef FILE;



class File
{
private:
	FILE* fp;
public:
	void SetFile(std::string path);
	template <typename T>
	void LoadFile(T* buffer, int readcount = 1,int size = -1);
	void SeekFile(int seeksize);
	void Close();
	File(std::string path);
	~File();
};

template <typename T>
void File::LoadFile(T* buffer, int readcount, int size)
{
	if (size == -1) size = sizeof(*buffer);
	fread(buffer, size, readcount, fp);
};