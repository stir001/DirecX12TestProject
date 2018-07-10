#pragma once
#include <vector>
#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include <string>
#include <dxgi1_4.h>
#include <functional>


class SwapChainObject;
class Dx12DescriptorHeapObject;


typedef std::vector<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>>& CmdListsArg_t;
typedef std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& RTResourcesArg_t;
typedef Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& RTDescHeapArg_t;

/**
*コマンドリストのComPtrの参照
*(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&)を引数に持つラムダ式の定義のtypedef
*初期化時にしたい実行コードをここに入れる
*D3D12_CPU_DESCRIPTOR_HANDLE& は設定されているレンダーターゲットの最初のポインタが入る
*中でレンダーターゲットのセットをすることを想定している
*/
typedef std::function<void(CmdListsArg_t, RTResourcesArg_t, RTDescHeapArg_t)> InitFunctionObject_t;
typedef std::function<void(CmdListsArg_t, RTResourcesArg_t)> LastFunctionObject_t;

/**
*レンダリングパスが順番に呼ばれることを保証する
*すべてのパスが終了した後コマンドリストをリセットする
*/
class RenderingPathManager
{
public:
	~RenderingPathManager();

	struct RenderTargetViewObject
	{
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resource;
		std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> cpuhandles;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescHeap;//rendertargetsのDescheap
	};

	struct RenderingPathObject
	{
		std::string pathName;
		RenderTargetViewObject rendertargets;
		InitFunctionObject_t initFunc;
		LastFunctionObject_t lastFunc;
		std::vector<std::string> cmdListName;
		std::vector<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>> cmdList;
		std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> allocator;
		bool isActive;
	};

	static RenderingPathManager* Instance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new RenderingPathManager();
		}
		return mInstance;
	}

	static void Destroy()
	{
		if (mInstance != nullptr)
		{
			delete mInstance;
			mInstance = nullptr;
		}
	}

	/**
	*RenderingManagerの初期化処理
	*/
	void Init(Microsoft::WRL::ComPtr<ID3D12Device>& dev, Microsoft::WRL::ComPtr<IDXGIFactory4>& factory, HWND hwnd);

	void Render();

	/**
	*パス番号は0から
	*パスのインデックスからそのパスのコマンドを積むべきコマンドリストを取得する
	*DeleteRenderingPathメソッドを呼んだ後だとインデックスが崩れるがコマンドリスト自体は変化しない
	*/
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetRenderingPathCommandList(unsigned int pathIndex) const;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetRenderingPathCommandList(unsigned int pathIndex, unsigned int cmdListIndex) const;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetRenderingPathCommandList(unsigned int pathIndex, const std::string& cmdListName) const;

	/**
	*パスの名前からそのパスのコマンドを積むべきコマンドリストを取得する
	*DeleteRenderingPathメソッドを呼んだ後だとインデックスが崩れるがコマンドリスト自体は変化しない
	*/
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetRenderingPathCommandList(const std::string& pathName) const;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetRenderingPathCommandList(const std::string& pathName, unsigned int cmdListIndex) const;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetRenderingPathCommandList(const std::string& pathName, const std::string& cmdListName) const;

	/**
	*パスの名前からそのパスのインデックスを取得する
	*レンダリングパスのインデックスを取得する
	*DeleteRenderingPathメソッドを呼んだ後だとインデックスが崩れるので注意
	*/
	unsigned int GetRenderingPathIndex(const std::string& pathName) const;

	/**
	*レンダーターゲットのCPUHANDLE,DESCRIPTOR_HEAP,ID3D12Resourceが入っている構造体の参照を返す
	*CPUHANDLEとID3D12Resrouceの順番は対応している
	*DeleteRenderingPathメソッドを呼んだ後だとインデックスが崩れるので注意
	*/
	const RenderTargetViewObject& GetRenderTargetViews(unsigned int pathIndex) const;

	void CreateDummyRenderTarget(Microsoft::WRL::ComPtr<ID3D12Resource>& resource);

	/**
	*中でコマンドリスト生成
	*レンダリングパスの名前入れる
	*引数に返り値用のパスを入れる
	*レンダリングパス名が同じだと追加されない
	*追加できないときout_PathIndexはUINT_MAXを返す
	*initFuncに毎フレームの初期化時にしてほしい処理を入れたラムダ式を登録することで
	*毎フレーム呼び出されることを保証する
	*initFuncの実行の後にレンダーターゲットをセットする
	*外で別の使い方で生成したID3D12Resourceを渡すとそれに対応したRenderTargetViewを作る
	*/
	bool AddRenderPath(const std::string& pathName, unsigned int& out_PathIndex, const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& resources,bool isActive = true);

	bool AddParallelCommandList(const std::string& pathName, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& out_cmdList, const std::string& cmdListName = "");
	bool AddParallelCommandList(unsigned int pathIndex, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& out_cmdList, const std::string& cmdListName = "");

	bool InsertRenderPath(const std::string& pathName, unsigned int insertPathIndex, const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& resources, bool isActive = true);

	bool SetInitFunction(const std::string& pathName, const InitFunctionObject_t& initFunc);
	bool SetInitFunction(unsigned int pathIndex, const InitFunctionObject_t& initFunc);

	bool SetLastFunction(const std::string& pathName, const LastFunctionObject_t& lastFunc);
	bool SetLastFunction(unsigned int pathIndex, const LastFunctionObject_t& lastFunc);

	/**
	*指定したレンダリングパスのコマンドリストをパス上から削除する
	*これを実行した後はパスのインデックスが崩れるので注意
	*/
	bool DeleteRenderingPath(unsigned int pathIndex);
	bool DeleteRenderingPath(const std::string& pathName);

	bool DeleteRenderingPathCommandListALL(unsigned int pathIndex);
	bool DeleteRenderingPathCommandListALL(const std::string& pathName);

	bool DeleteRenderingPathCommandList(unsigned int pathIndex, unsigned int cmdListIndex);
	bool DeleteRenderingPathCommandList(unsigned int pathIndex, const std::string& cmdListName);
	bool DeleteRenderingPathCommandList(const std::string& pathName, unsigned int cmdListIndex);
	bool DeleteRenderingPathCommandList(const std::string& pathName, const std::string& cmdListName);
	/**
	*パスの全体の数を取得
	*/
	unsigned int GetNumCuurentPath() const;

	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentRTVHeapHandle() const;
	std::shared_ptr<Dx12DescriptorHeapObject> GetCurrentRTVDescHeap() const;

	Microsoft::WRL::ComPtr<ID3D12Resource> GetCurrentRenderTarget() const;

	void SetRenderTargetView(unsigned int pathIndex, const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& resources);
	void SetRenderTargetView(const std::string& pathName, const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& resources);
	
	void DeleteRenderTargetView(const std::string& pathName);
	void DeleteRenderTargetView(unsigned int pathIndex);

	void SetIsActiveValue(unsigned int renderingPath, bool isActive);
	void SetIsActiveValue(std::string pathName, bool isActive);

	void AllPathClear();

private:
	RenderingPathManager();
	RenderingPathManager(const RenderingPathManager&) = delete;
	RenderingPathManager(const RenderingPathManager&&) = delete;
	RenderingPathManager& operator=(const RenderingPathManager&) = delete;
	RenderingPathManager& operator=(const RenderingPathManager&&) = delete;

	static RenderingPathManager* mInstance;
	
	std::vector<RenderingPathObject> mRenderingPathObjects;

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mRenderCmdAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mRenderCmdList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCmdQueue;
	Microsoft::WRL::ComPtr<ID3D12Device> mDevice;
	
	std::shared_ptr<SwapChainObject> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
	UINT64 mFenceValue;

	unsigned int mWidth;
	unsigned int mHeight;
	
	unsigned int AddRenderingPathObject(const std::string& pathName,const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& resources);

	void WaitCmdQueue();
	void CopyLastPathRenderTarget();
	void ExcuteCommnadLists(const std::vector<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>>& cmdLists);
	void ExcuteOneCommnadList(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
	void ClosePathCommandLists(RenderingPathObject& pathObj);
	void ResetPathCommandLists(RenderingPathObject& pathObj);
	void ExcuteInitFaunc(RenderingPathObject& pathObj);
	void CreateRenderPathObject(RenderingPathObject& pathObj, const std::string& pathNamet, const std::string& cmdListName = "");
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetPathCommandList(const RenderingPathObject& pathObj, const std::string& cmdListName) const;
	unsigned int GetPathCommnadnListIndex(const RenderingPathObject& pathObj, const std::string& cmdListName) const;
	void AddCmdObjcets(RenderingPathObject& pathObj, const std::string& cmdListName = "");
};

