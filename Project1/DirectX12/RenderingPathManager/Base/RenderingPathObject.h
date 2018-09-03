#pragma once
#include <wrl.h>
struct ID3D12CommandQueue;

class RenderingPathObject
{
public:
	RenderingPathObject();
	virtual ~RenderingPathObject();

	virtual void FirstUpdate() = 0;	//各フレームの一番最初に実行される
	virtual void LastUpdate() = 0;	//直前のRenderingPathObjectのExcutePathが実行されたあとに実行される
	virtual void ExcutePath(Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue) = 0;	//コマンドリストの実行をする。GPU待ちの責任は負わない
	virtual void ResetCommandList() = 0;	//ExcutePathの実行後に実行される。CommnadListのリセットをする
	virtual void SetActive(bool isActive);	//このパスを有効にするか無効にするかの設定を行う
	virtual bool IsActive() const;			//このパスが有効か無効かを返す:true 有効, false 無効
protected:
private:
	bool mIsActive;
};

