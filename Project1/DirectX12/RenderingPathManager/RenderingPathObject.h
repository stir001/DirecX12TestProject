#pragma once
class RenderingPathObject
{
public:
	RenderingPathObject();
	virtual ~RenderingPathObject();

	virtual void FirstUpdate() = 0;	//各フレームの一番最初に実行されるアップデート
	virtual void LastUpdate() = 0;	//レンダリングする直前に実行されるアップデート

protected:

};

