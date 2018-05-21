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

struct ActionImageData
{
	Rect imageRect;//画像切り抜き情報
	DirectX::XMFLOAT2 pivot;
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
