#include "RenderingPathManager.h"
#include "CharToWChar.h"
#include "SwapChainObject.h"
#include "Dx12DescriptorHeapObject.h"

#include <d3dx12.h>
#include <algorithm>
#include <assert.h>

RenderingPathManager* RenderingPathManager::mInstance = nullptr;

RenderingPathManager::RenderingPathManager()
{
}


RenderingPathManager::~RenderingPathManager()
{
	mRenderingPathObjects.clear();
	mSwapChain.reset();
	mFence.Reset();
}

void RenderingPathManager::Init(Microsoft::WRL::ComPtr<ID3D12Device>& dev, Microsoft::WRL::ComPtr<IDXGIFactory4>& factory, HWND hwnd)
{
	mRenderingPathObjects.clear();
	mCmdQueue.Reset();

	mDevice = dev;
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mRenderCmdAllocator));
	mRenderCmdAllocator->SetName(L"RenderingCommandAllocator");

	mDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mRenderCmdAllocator.Get(), nullptr, IID_PPV_ARGS(&mRenderCmdList));
	mRenderCmdList->SetName(L"RenderingCommandList");

	mDevice->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&mCmdQueue));
	mCmdQueue->SetName(L"RenderingCommandQueue");

	mSwapChain.reset(new SwapChainObject(hwnd, mDevice, factory, mCmdQueue));

	DXGI_SWAP_CHAIN_DESC desc;
	mSwapChain->GetSwapChain()->GetDesc(&desc);
	mWidth = desc.BufferDesc.Width;
	mHeight = desc.BufferDesc.Height;

	mDevice->CreateFence(mFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));
	mFence->SetName(L"RenderingManagerFence");

	InitFunctionObject_t nonFunc = [](CmdListsArg_t, RTResourcesArg_t, RTDescHeapArg_t) {};
	LastFunctionObject_t nonlastFunc = [](CmdListsArg_t, RTResourcesArg_t) {};

	Microsoft::WRL::ComPtr<ID3D12Resource> resoruce;//シェーダーリソースビューとして作成してから渡す
	CreateDummyRenderTarget(resoruce);


	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> firstTarget;
	firstTarget.push_back(resoruce);

	unsigned int index = AddRenderingPathObject("FirstPath", firstTarget);

}

void RenderingPathManager::Render()
{
	for (auto& pathObj : mRenderingPathObjects)
	{
		if (!pathObj.isActive)
		{
			continue;
		}
		pathObj.lastFunc(pathObj.cmdList, pathObj.rendertargets.resource);
		ExcuteCommnadLists(pathObj.cmdList);

		WaitCmdQueue();

		ResetPathCommandLists(pathObj);
	}

	CopyLastPathRenderTarget();

	HRESULT result = mSwapChain->SwapChainPresent(1, 0);
	assert(!FAILED(result));

	float clearColor[4] = { 0.0f,0.0f,0.0f,1.0f };
	D3D12_RECT rect = {};
	rect.right = mWidth;
	rect.bottom = mHeight;

	mRenderCmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mSwapChain->GetCurrentRenderTarget().Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	mRenderCmdList->ClearRenderTargetView(mSwapChain->GetCurrentRTVHeap(), clearColor, 1, &rect);

	mRenderCmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mSwapChain->GetCurrentRenderTarget().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	for (auto& pathObj : mRenderingPathObjects)
	{
		if (!pathObj.isActive)
		{
			continue;
		}
		ExcuteInitFaunc(pathObj);
	}
}

void RenderingPathManager::CopyLastPathRenderTarget()
{
	auto& lastPath = mRenderingPathObjects.back();
	D3D12_RESOURCE_STATES lastPathBeforeState = D3D12_RESOURCE_STATE_RENDER_TARGET;
	D3D12_RESOURCE_STATES lastPathAfterState = D3D12_RESOURCE_STATE_COPY_SOURCE;
	Microsoft::WRL::ComPtr<ID3D12Resource> lastPathResrouce = lastPath.rendertargets.resource[0];
	mRenderCmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(lastPathResrouce.Get(), lastPathBeforeState, lastPathAfterState));

	D3D12_RESOURCE_STATES swapChainBeforeState = D3D12_RESOURCE_STATE_PRESENT;
	D3D12_RESOURCE_STATES swapChainAfterState = D3D12_RESOURCE_STATE_COPY_DEST;
	Microsoft::WRL::ComPtr<ID3D12Resource> swapChainResrouce = mSwapChain->GetCurrentRenderTarget();
	mRenderCmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(swapChainResrouce.Get(), swapChainBeforeState, swapChainAfterState));

	mRenderCmdList->CopyResource(mSwapChain->GetCurrentRenderTarget().Get(), lastPath.rendertargets.resource[0].Get());

	mRenderCmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(lastPathResrouce.Get(), lastPathAfterState, lastPathBeforeState));
	mRenderCmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(swapChainResrouce.Get(), swapChainAfterState, D3D12_RESOURCE_STATE_RENDER_TARGET));

	mRenderCmdList->OMSetRenderTargets(1, &mSwapChain->GetCurrentRTVHeap(), false, nullptr);

	mRenderCmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(swapChainResrouce.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, swapChainBeforeState));

	ExcuteOneCommnadList(mRenderCmdList);

	WaitCmdQueue();

	mRenderCmdAllocator->Reset();
	mRenderCmdList->Reset(mRenderCmdAllocator.Get(), nullptr);

}

unsigned int RenderingPathManager::GetNumCuurentPath() const
{
	return mRenderingPathObjects.size();
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> RenderingPathManager::GetRenderingPathCommandList(unsigned int pathIndex) const
{
	if (pathIndex < mRenderingPathObjects.size())
	{
		return mRenderingPathObjects[pathIndex].cmdList[0];
	}
	return nullptr;
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> RenderingPathManager::GetRenderingPathCommandList(unsigned int pathIndex, unsigned int cmdListIndex) const
{
	if (pathIndex < mRenderingPathObjects.size())
	{
		return mRenderingPathObjects[pathIndex].cmdList[cmdListIndex];
	}
	return nullptr;
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> RenderingPathManager::GetRenderingPathCommandList(unsigned int pathIndex, const std::string & cmdListName) const
{
	if (pathIndex < mRenderingPathObjects.size())
	{
		return GetPathCommandList(mRenderingPathObjects[pathIndex], cmdListName);
	}
	return nullptr;
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> RenderingPathManager::GetRenderingPathCommandList(const std::string& pathName) const
{
	unsigned int index = GetRenderingPathIndex(pathName);
	if (index != UINT_MAX)
	{
		return mRenderingPathObjects[index].cmdList[0];
	}

	return nullptr;
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> RenderingPathManager::GetRenderingPathCommandList(const std::string & pathName, unsigned int cmdListIndex) const
{
	unsigned int index = GetRenderingPathIndex(pathName);
	if (index != UINT_MAX)
	{
		return mRenderingPathObjects[index].cmdList[cmdListIndex];
	}
	return nullptr;
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> RenderingPathManager::GetRenderingPathCommandList(const std::string & pathName, const std::string & cmdListName) const
{
	unsigned int index = GetRenderingPathIndex(pathName);
	if (index != UINT_MAX)
	{
		return GetPathCommandList(mRenderingPathObjects[index], cmdListName);
	}
	return nullptr;
}


bool RenderingPathManager::AddRenderPath(const std::string& pathName, unsigned int& out_PathIndex, const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& resources, bool isActive)
{
	auto& itr = std::find_if(mRenderingPathObjects.begin(), mRenderingPathObjects.end(), [&pathName](const RenderingPathObject& value) {return value.pathName == pathName; });
	if (itr != mRenderingPathObjects.end())
	{
		out_PathIndex = UINT_MAX;
		return false;
	}
	out_PathIndex = AddRenderingPathObject(pathName, resources);
	mRenderingPathObjects[out_PathIndex].isActive = isActive;
	return true;
}

bool RenderingPathManager::AddParallelCommandList(const std::string& pathName, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& out_cmdList, const std::string& cmdListName)
{
	unsigned int index = GetRenderingPathIndex(pathName);
	if (index != UINT_MAX)
	{
		mRenderingPathObjects[index].cmdListName.push_back(cmdListName);
		AddCmdObjcets(mRenderingPathObjects[index], cmdListName);
		out_cmdList =  mRenderingPathObjects[index].cmdList.back();
		return true;
	}
	return false;
}

bool RenderingPathManager::AddParallelCommandList(unsigned int pathIndex, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& out_cmdList, const std::string& cmdListName)
{
	if (pathIndex < mRenderingPathObjects.size())
	{
		mRenderingPathObjects[pathIndex].cmdListName.push_back(cmdListName);
		AddCmdObjcets(mRenderingPathObjects[pathIndex], cmdListName);
		return true;
	}
	return false;
}


bool RenderingPathManager::InsertRenderPath(const std::string& pathName, unsigned int insertPathIndex, const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& resources, bool isActive)
{
	if (insertPathIndex <= mRenderingPathObjects.size())
	{
		RenderingPathObject insertPath;
		CreateRenderPathObject(insertPath, pathName);
		auto itr = mRenderingPathObjects.begin();
		insertPath.isActive = isActive;
		mRenderingPathObjects.insert((itr + insertPathIndex), insertPath);
		return true;
	}
	return false;
}

bool RenderingPathManager::SetInitFunction(const std::string& pathName, const InitFunctionObject_t& initFunc)
{
	unsigned int index = GetRenderingPathIndex(pathName);
	if (index != UINT_MAX)
	{
		RenderingPathObject& obj = mRenderingPathObjects[index];
		ClosePathCommandLists(obj);
		ResetPathCommandLists(obj);

		obj.initFunc = initFunc;

		if (obj.isActive)
		{
			ExcuteInitFaunc(obj);
		}
		return true;
	}

	return false;
}

bool RenderingPathManager::SetInitFunction(unsigned int pathIndex, const InitFunctionObject_t& initFunc)
{
	if (pathIndex < mRenderingPathObjects.size())
	{
		RenderingPathObject& obj = mRenderingPathObjects[pathIndex];

		ClosePathCommandLists(obj);
		ResetPathCommandLists(obj);

		obj.initFunc = initFunc;

		if (obj.isActive)
		{
			ExcuteInitFaunc(obj);
		}
		return true;
	}

	return false;
}
bool RenderingPathManager::SetLastFunction(const std::string& pathName, const LastFunctionObject_t& lastFunc)
{
	unsigned int index = GetRenderingPathIndex(pathName);
	if (index != UINT_MAX)
	{
		mRenderingPathObjects[index].lastFunc = lastFunc;
		return true;
	}
	return false;
}

bool RenderingPathManager::SetLastFunction(unsigned int pathIndex, const LastFunctionObject_t& lastFunc)
{
	if (pathIndex < mRenderingPathObjects.size())
	{
		mRenderingPathObjects[pathIndex].lastFunc = lastFunc;
	}
	return false;
}

unsigned int RenderingPathManager::AddRenderingPathObject(const std::string& pathName, const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& resources)
{
	RenderingPathObject addPath;
	CreateRenderPathObject(addPath, pathName);

	mRenderingPathObjects.push_back(addPath);

	SetRenderTargetView(mRenderingPathObjects.size() - 1, resources);

	return mRenderingPathObjects.size() - 1;
}

unsigned int RenderingPathManager::GetRenderingPathIndex(const std::string& pathName) const
{
	for (unsigned int i = 0; i < mRenderingPathObjects.size(); ++i)
	{
		if (mRenderingPathObjects[i].pathName == pathName)
		{
			return i;
		}
	}
	return UINT_MAX;
}

bool RenderingPathManager::DeleteRenderingPath(unsigned int pathIndex)
{
	if (mRenderingPathObjects.size() > pathIndex)
	{
		mRenderingPathObjects.erase(mRenderingPathObjects.begin() + pathIndex);
		return true;
	}

	return false;
}

bool RenderingPathManager::DeleteRenderingPath(const std::string & pathName)
{
	unsigned int index = GetRenderingPathIndex(pathName);
	if (index != UINT_MAX)
	{
		mRenderingPathObjects.erase(mRenderingPathObjects.begin() + index);
		return true;
	}
	return false;
}

bool RenderingPathManager::DeleteRenderingPathCommandListALL(unsigned int pathIndex)
{
	if (pathIndex < mRenderingPathObjects.size())
	{
		ClosePathCommandLists(mRenderingPathObjects[pathIndex]);
		ResetPathCommandLists(mRenderingPathObjects[pathIndex]);
		mRenderingPathObjects[pathIndex].allocator.clear();
		mRenderingPathObjects[pathIndex].cmdList.clear();
	}
	return false;
}

bool RenderingPathManager::DeleteRenderingPathCommandListALL(const std::string & pathName)
{
	unsigned int index = GetRenderingPathIndex(pathName);
	if (index != UINT_MAX)
	{
		ClosePathCommandLists(mRenderingPathObjects[index]);
		ResetPathCommandLists(mRenderingPathObjects[index]);
		mRenderingPathObjects[index].allocator.clear();
		mRenderingPathObjects[index].cmdList.clear();
	}
	return false;
}

bool RenderingPathManager::DeleteRenderingPathCommandList(unsigned int pathIndex, unsigned int cmdListIndex)
{
	if (pathIndex != UINT_MAX)
	{
		auto& cmdLists = mRenderingPathObjects[pathIndex].cmdList;
		cmdLists.erase(cmdLists.begin() + cmdListIndex);
		return true;
	}
	return false;
}

bool RenderingPathManager::DeleteRenderingPathCommandList(unsigned int pathIndex, const std::string& cmdListName)
{
	if (pathIndex != UINT_MAX)
	{
		unsigned int cmdListIndex = GetPathCommnadnListIndex(mRenderingPathObjects[pathIndex], cmdListName);
		if (cmdListIndex != UINT_MAX)
		{
			auto& cmdLists = mRenderingPathObjects[pathIndex].cmdList;
			cmdLists.erase(cmdLists.begin() + cmdListIndex);
			return true;
		}
	}

	return false;
}

bool RenderingPathManager::DeleteRenderingPathCommandList(const std::string& pathName, unsigned int cmdListIndex)
{
	unsigned int pathIndex = GetRenderingPathIndex(pathName);
	if (pathIndex != UINT_MAX)
	{
		auto& cmdLists = mRenderingPathObjects[pathIndex].cmdList;
		cmdLists.erase(cmdLists.begin() + cmdListIndex);
		return true;
	}

	return false;
}

bool RenderingPathManager::DeleteRenderingPathCommandList(const std::string& pathName, const std::string& cmdListName)
{
	unsigned int pathIndex = GetRenderingPathIndex(pathName);
	if (pathIndex != UINT_MAX)
	{
		unsigned int cmdListIndex = GetPathCommnadnListIndex(mRenderingPathObjects[pathIndex], cmdListName);
		if (cmdListIndex != UINT_MAX)
		{
			auto& cmdLists = mRenderingPathObjects[pathIndex].cmdList;
			cmdLists.erase(cmdLists.begin() + cmdListIndex);
			return true;
		}
	}
	return false;
}

D3D12_CPU_DESCRIPTOR_HANDLE RenderingPathManager::GetCurrentRTVHeapHandle() const
{
	return mSwapChain->GetCurrentRTVHeap();
}

void RenderingPathManager::WaitCmdQueue()
{
	mCmdQueue->Signal(mFence.Get(), ++mFenceValue);
	UINT64 value = mFence->GetCompletedValue();
	UINT64 u64max = UINT64_MAX;
	while (value != mFenceValue)
	{
		value = mFence->GetCompletedValue();
		if (value == UINT64_MAX)
		{
			HRESULT result = mDevice->GetDeviceRemovedReason();
			assert(!FAILED(result));
		}
	}
}

std::shared_ptr<Dx12DescriptorHeapObject> RenderingPathManager::GetCurrentRTVDescHeap() const
{
	return mSwapChain->GetDescriptorHeap();
}

Microsoft::WRL::ComPtr<ID3D12Resource> RenderingPathManager::GetCurrentRenderTarget() const
{
	return mSwapChain->GetCurrentRenderTarget();
}

void RenderingPathManager::SetRenderTargetView(unsigned int pathIndex, const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& resources)
{
	if (resources.size() <= 0)
	{
		return;
	}

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = resources.size();
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescHeap;

	mDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvDescHeap));


	auto& setPathRT = mRenderingPathObjects[pathIndex].rendertargets;

	setPathRT.cpuhandles.clear();
	setPathRT.rtvDescHeap = (rtvDescHeap);
	setPathRT.resource.clear();

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = rtvDescHeap->GetCPUDescriptorHandleForHeapStart();

	UINT incrementSize = mDevice->GetDescriptorHandleIncrementSize(rtvDescHeap->GetDesc().Type);

	setPathRT.cpuhandles.reserve(resources.size());
	for (auto& rsc : resources)
	{
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc;
		rtvDesc.Format = rsc->GetDesc().Format;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
		rtvDesc.Texture2D.PlaneSlice = 0;

		mDevice->CreateRenderTargetView(rsc.Get(), &rtvDesc, cpuHandle);
		setPathRT.cpuhandles.push_back(cpuHandle);
		setPathRT.resource.push_back(rsc);
		cpuHandle.ptr += incrementSize;
	}
}

void RenderingPathManager::SetRenderTargetView(const std::string& pathName, const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& resources)
{
	unsigned int index = GetRenderingPathIndex(pathName);
	SetRenderTargetView(index, resources);
}

void RenderingPathManager::DeleteRenderTargetView(const std::string& pathName)
{
	unsigned int index = GetRenderingPathIndex(pathName);
	DeleteRenderTargetView(index);
}

void RenderingPathManager::DeleteRenderTargetView(unsigned int pathIndex)
{
	if (pathIndex >= mRenderingPathObjects.size())
	{
		mRenderingPathObjects[pathIndex].rendertargets.cpuhandles.clear();
		mRenderingPathObjects[pathIndex].rendertargets.resource.clear();
		mRenderingPathObjects[pathIndex].rendertargets.rtvDescHeap.Reset();
	}
}

void RenderingPathManager::SetIsActiveValue(unsigned int pathIndex, bool isActive)
{
	if (mRenderingPathObjects.size() >= pathIndex)
	{
		auto& pathObj = mRenderingPathObjects[pathIndex];
		if (pathObj.isActive == isActive)
		{
			return;
		}
		pathObj.isActive = isActive;
		if (isActive)
		{
			ExcuteInitFaunc(pathObj);
		}
		else
		{
			ClosePathCommandLists(pathObj);
			ResetPathCommandLists(pathObj);
		}
	}
}

void RenderingPathManager::SetIsActiveValue(std::string pathName, bool isActive)
{
	unsigned int index = GetRenderingPathIndex(pathName);
	if (index != UINT_MAX)
	{
		auto& pathObj = mRenderingPathObjects[index];
		pathObj.isActive = isActive;
		if (isActive)
		{
			ExcuteInitFaunc(pathObj);
		}
		else
		{
			ClosePathCommandLists(pathObj);
			ResetPathCommandLists(pathObj);
		}
	}
}

void RenderingPathManager::AllPathClear()
{
	mRenderingPathObjects.clear();
	std::vector<RenderingPathObject>().swap(mRenderingPathObjects);
}

const RenderingPathManager::RenderTargetViewObject& RenderingPathManager::GetRenderTargetViews(unsigned int pathIndex) const
{
	return mRenderingPathObjects[pathIndex].rendertargets;
}

void RenderingPathManager::ExcuteCommnadLists(const std::vector<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>>& incmdLists)
{
	std::vector<ID3D12CommandList*> t_cmdLists;
	t_cmdLists.resize(incmdLists.size());
	for (unsigned int i = 0; i < incmdLists.size(); ++i)
	{
		t_cmdLists[i] = incmdLists[i].Get();
		incmdLists[i]->Close();
	}
	mCmdQueue->ExecuteCommandLists(t_cmdLists.size(), &t_cmdLists[0]);
}

void RenderingPathManager::ExcuteOneCommnadList(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
{
	ID3D12CommandList* t_cmdList = cmdList.Get();
	cmdList->Close();
	mCmdQueue->ExecuteCommandLists(1, &t_cmdList);
}

void RenderingPathManager::CreateDummyRenderTarget(Microsoft::WRL::ComPtr<ID3D12Resource>& resource)
{
	D3D12_RESOURCE_DESC desc;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Alignment = 0;
	desc.Width = mSwapChain->GetCurrentRenderTarget()->GetDesc().Width;
	desc.Height = mSwapChain->GetCurrentRenderTarget()->GetDesc().Height;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProp.CreationNodeMask = 1;
	heapProp.VisibleNodeMask = 1;

	D3D12_CLEAR_VALUE value;
	value.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	value.Color[0] = 0.5f;
	value.Color[1] = 0.5f;
	value.Color[2] = 0.5f;
	value.Color[3] = 1.0f;

	HRESULT result = mDevice->CreateCommittedResource(&heapProp
		, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE
		, &desc
		, D3D12_RESOURCE_STATE_RENDER_TARGET
		, &value
		, IID_PPV_ARGS(&resource));

	resource->SetName(L"DummyResource");
}

void RenderingPathManager::ExcuteInitFaunc(RenderingPathObject& pathObj)
{
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
	if (pathObj.rendertargets.rtvDescHeap != nullptr)
	{
		descHeap = pathObj.rendertargets.rtvDescHeap;
	}
	pathObj.initFunc(pathObj.cmdList, pathObj.rendertargets.resource, descHeap);
}

void RenderingPathManager::CreateRenderPathObject(RenderingPathObject& pathObj, const std::string& pathName, const std::string& cmdListName)
{
	pathObj.pathName = pathName;
	pathObj.cmdListName.push_back(pathName);

	AddCmdObjcets(pathObj, cmdListName);

	pathObj.initFunc = [](CmdListsArg_t, RTResourcesArg_t, RTDescHeapArg_t) {};
	pathObj.lastFunc = [](CmdListsArg_t, RTResourcesArg_t) {};

}

void RenderingPathManager::ClosePathCommandLists(RenderingPathObject& pathObj)
{
	for (unsigned int i = 0; i < pathObj.cmdList.size(); ++i)
	{
		pathObj.cmdList[i]->Close();
	}
}

void RenderingPathManager::ResetPathCommandLists(RenderingPathObject& pathObj)
{
	for (unsigned int i = 0; i < pathObj.cmdList.size(); ++i)
	{
		pathObj.allocator[i]->Reset();
		pathObj.cmdList[i]->Reset(pathObj.allocator[i].Get(), nullptr);
	}
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> RenderingPathManager::GetPathCommandList(const RenderingPathObject& pathObj, const std::string& cmdListName) const
{
	for (unsigned int i = 0; i < pathObj.cmdListName.size(); ++i)
	{
		if (pathObj.cmdListName[i] == cmdListName)
		{
			return pathObj.cmdList[i];
		}
	}
	return nullptr;
}

unsigned int RenderingPathManager::GetPathCommnadnListIndex(const RenderingPathObject& pathObj, const std::string& cmdListName) const
{
	unsigned int rtn = 0;
	for (auto& cmdName : pathObj.cmdListName)
	{
		if (cmdName == cmdListName)
		{
			return rtn;
		}
		++rtn;
	}

	return UINT_MAX;
}

void RenderingPathManager::AddCmdObjcets(RenderingPathObject& pathObj, const std::string& cmdListName)
{
	wchar_t* nameBuffer = nullptr;
	wchar_t* cmdName = nullptr;
	bool isName = false;
	std::wstring pathnameStr;

	wchar_t* cmdNostr = nullptr;
	std::string number;

	unsigned int secondNum = (pathObj.cmdList.size() / 10) % 10;
	number += secondNum + '0';
	unsigned int firstnum = pathObj.cmdList.size() % 10;
	number += firstnum + '0';
	ToWChar(&cmdNostr, number);

	ToWChar(&nameBuffer, pathObj.pathName);

	if (isName = cmdListName != "")
	{
		ToWChar(&cmdName, cmdListName);
	}

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAllocator;
	mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator));
	pathnameStr = nameBuffer;
	if (isName)
	{
		pathnameStr += cmdName;
	}
	pathnameStr += L"CommandAllocator";
	pathnameStr += cmdNostr;
	pathnameStr += L"\0";
	cmdAllocator->SetName(pathnameStr.data());
	pathObj.allocator.push_back(cmdAllocator);
	pathnameStr.clear();

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList;
	mDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&cmdList));
	pathnameStr = nameBuffer;
	if (isName)
	{
		pathnameStr += cmdName;
	}
	pathnameStr += L"CommandList";
	pathnameStr += cmdNostr;
	pathnameStr += L"\0";
	cmdList->SetName(pathnameStr.data());
	pathObj.cmdList.push_back(cmdList);

	pathnameStr.clear();
	delete nameBuffer;
	if (isName)
	{
		delete cmdName;
	}
}