#include "DirectX12Buffer.h"



DirectX12Buffer::DirectX12Buffer() : buffer(nullptr),elementBuffer(nullptr)
{
}


DirectX12Buffer::~DirectX12Buffer()
{
	buffer->Release();
}

void DirectX12Buffer::WriteBuffer(void* pData, unsigned int dataSize)
{
	memcpy(elementBuffer, pData, dataSize);
	//buffer->Unmap(0, nullptr);
}

void DirectX12Buffer::Map()
{
	buffer->Map(0, nullptr, &elementBuffer);
}

void DirectX12Buffer::WriteBuffer256Alignment(void* pData, unsigned int datasize, unsigned int datacount)
{
	//void* adress;
	//void* dataadrees;
	for (unsigned int i = 0; i < datacount; i++)
	{
		memcpy((void*)((char*)elementBuffer + i * ((datasize + 0xff) & ~0xff)), (void*)((char*)pData + i * datasize), datasize);
		//adress = (void*)((char*)elementBuffer + i * ((datasize + 0xff) & ~0xff));
		//dataadrees = (void*)((char*)pData + i * datasize);
	}
}

