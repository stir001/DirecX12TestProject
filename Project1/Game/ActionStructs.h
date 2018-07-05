#pragma once
#include "Rect.h"
#include <vector>
#include <string>

enum eACTION_RECT_TYPE
{
	eACTION_RECT_TYPE_ANCHOR,//�A���J�[(0)
	eACTION_RECT_TYPE_ATTACK,//�U����`(1)
	eACTION_RECT_TYPE_DAMAGE//�����`(2)
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

//���㌴�_�� +x���E +y���� �ɂȂ��Ă���
struct ActionImageData
{
	Rect imageRect;//�摜�؂蔲����� ���W�n����
	DirectX::XMFLOAT2 pivot;//���W�n����
	int duraction;
	std::vector<ActionRect> actionRects;//�������`���
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
