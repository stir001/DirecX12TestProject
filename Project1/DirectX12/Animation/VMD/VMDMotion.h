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
	unsigned int count;//モーションカウント
};

struct VMDMotionData
{
	char boneName[15];//ボーン名
	unsigned long frameNo;//フレーム番号
	DirectX::XMFLOAT3 location;//座標
	DirectX::XMFLOAT4 quoternion;//回転
	unsigned char interpolation[64];//保管パラメータ
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

