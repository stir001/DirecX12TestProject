#pragma once
#include <Dx12MSLib.h>

class TransParencyPass : RenderingPassObject
{
public:
	TransParencyPass(std::shared_ptr<Dx12DescriptorHeapObject>& rtvHeap,
		std::shared_ptr<Dx12DescriptorHeapObject>& depthHeap);
	~TransParencyPass();

	/**
	*	各フレームの一番最初に実行される
	*/
	void FirstUpdate();

	/**
	*	このパスのExecutePathが呼ばれる直前に実行される
	*	基本的にここでコマンドリストはcloseを呼ぶ
	*/
	void PreExecuteUpdate();

	/**
	*	コマンドリストの実行をする。GPU待ちの責任は負わない
	*/
	void ExecutePath(Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue);

	/**
	*	ExecutePathの実行後に実行される。CommnadListのリセットをする
	*/
	void ResetCommandList();

	/**
	*	コマンドリストを取得する
	*/
	std::shared_ptr<Dx12CommandList> GetCommandList();
private:
	std::shared_ptr<Dx12DescriptorHeapObject> mRTVDescHeap;
	std::shared_ptr<Dx12DescriptorHeapObject> mDepthHeap;
};

