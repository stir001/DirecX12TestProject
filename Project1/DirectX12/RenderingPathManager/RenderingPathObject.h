#pragma once
class RenderingPathObject
{
public:
	RenderingPathObject();
	virtual ~RenderingPathObject();

	virtual void FirstUpdate() = 0;	//�e�t���[���̈�ԍŏ��Ɏ��s�����A�b�v�f�[�g
	virtual void LastUpdate() = 0;	//�����_�����O���钼�O�Ɏ��s�����A�b�v�f�[�g

protected:

};

