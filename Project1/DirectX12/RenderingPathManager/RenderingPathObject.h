#pragma once
#include <d3d12.h>
#include <string>
#include <vector>

class Dx12CommandList;

class RenderingPathObject
{
public:
	RenderingPathObject();
	virtual ~RenderingPathObject();

	virtual void FirstUpdate() = 0;	//各フレームの一番最初に実行されるアップデート
	virtual void LastUpdate() = 0;	//レンダリングする直前に実行されるアップデート
	virtual const std::vector<std::shared_ptr<Dx12CommandList>>& GetCommnadLists() const;

protected:
	std::shared_ptr<Dx12CommandList>& CreateCommandList(D3D12_COMMAND_LIST_TYPE type, const std::string& name);
	std::vector<std::shared_ptr<Dx12CommandList>> mCmdLists;
};

