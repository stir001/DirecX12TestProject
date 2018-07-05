#pragma once
#include "Rect.h"
#include <vector>
#include <string>

enum eACTION_RECT_TYPE
{
	eACTION_RECT_TYPE_ANCHOR,//アンカー(0)
	eACTION_RECT_TYPE_ATTACK,//攻撃矩形(1)
	eACTION_RECT_TYPE_DAMAGE//やられ矩形(2)
};

struct ActionHeader
{
	float version;
	unsigned int fileNameSize;
	std::vector<char> imagefilename;
};

struct ActionRect
{
	eACTION_RECT_TYPE type;
	Rect rect;
};

//左上原点の +xが右 +yが下 になっている
struct ActionImageData
{
	Rect imageRect;//画像切り抜き情報 座標系注意
	DirectX::XMFLOAT2 pivot;//座標系注意
	int duraction;
	std::vector<ActionRect> actionRects;//あたり矩形情報
};

struct Action
{
	std::string actionName;
	bool isLoop;
	std::vector<ActionImageData> datas;
};

struct ActionData
{
	ActionHeader header;
	std::string relativePath;
	std::vector<Action> action;
};
