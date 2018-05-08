#pragma once
#include <d3d12.h>
#include "d3dx12.h"


class DirectX12Buffer
{
protected:
	ID3D12Resource* buffer;
	void* elementBuffer;
	void Map();
public:

	//@param pData �������ރf�[�^�̃|�C���^
	//@param dataSize data�S�̂̃o�C�g�P�ʂ̃T�C�Y
	//Map���ꂽ�̈�Ɉ�����data����������
	void WriteBuffer(const void* pData, unsigned int dataSize);

	//@param pData �f�[�^�̃|�C���^
	//@param datasize �f�[�^��̃T�C�Y
	//@param datacount �f�[�^�̑���
	void WriteBuffer256Alignment(const void* pData, unsigned int datasize, unsigned int datacount);

	DirectX12Buffer();
	~DirectX12Buffer();
};

