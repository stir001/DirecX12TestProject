#pragma once
#include "Animation.h"

#include <DirectXMath.h>
#include <map>
#include <vector>
#include <string>

class PMDModel;

struct VMDHeader
{
	unsigned char header[30];
	unsigned char modelName[20];
	unsigned int count;//���[�V�����J�E���g
};

struct VMDMotionData
{
	char boneName[15];//�{�[����
	unsigned long frameNo;//�t���[���ԍ�
	DirectX::XMFLOAT3 location;//���W
	DirectX::XMFLOAT4 quoternion;//��]
	unsigned char interpolation[64];//�ۊǃp�����[�^
};

struct VMDPose
{
	int frameNo;
	DirectX::XMFLOAT4 quoternion;
};

class VMDMotion :public Animation
{
	friend class VMDLoader;
	friend class VMDPlayer;
public:
	VMDMotion();
	~VMDMotion();
private:
	VMDHeader mHeader;
	std::vector<VMDMotionData> mMotions;
	std::map<std::string, std::vector<VMDPose>> mPoses;
};

