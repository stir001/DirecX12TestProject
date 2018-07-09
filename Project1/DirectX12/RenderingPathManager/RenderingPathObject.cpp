#include "RenderingPathObject.h"
#include "Dx12CommandList.h"
#include "Dx12Ctrl.h"

RenderingPathObject::RenderingPathObject()
{
}


RenderingPathObject::~RenderingPathObject()
{
}

const std::vector<std::shared_ptr<Dx12CommandList>>& RenderingPathObject::GetCommnadLists() const
{
	return mCmdLists;
}

std::shared_ptr<Dx12CommandList>& RenderingPathObject::CreateCommandList(D3D12_COMMAND_LIST_TYPE type, const std::string & name)
{
	std::shared_ptr<Dx12CommandList> cmdList(new Dx12CommandList(name, Dx12Ctrl::Instance()->GetDev(), type));
	mCmdLists.push_back(cmdList);
	return mCmdLists.back();
}
