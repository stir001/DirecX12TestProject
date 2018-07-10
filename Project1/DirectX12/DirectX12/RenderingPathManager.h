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
*�R�}���h���X�g��ComPtr�̎Q��
*(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&)�������Ɏ������_���̒�`��typedef
*���������ɂ��������s�R�[�h�������ɓ����
*D3D12_CPU_DESCRIPTOR_HANDLE& �͐ݒ肳��Ă��郌���_�[�^�[�Q�b�g�̍ŏ��̃|�C���^������
*���Ń����_�[�^�[�Q�b�g�̃Z�b�g�����邱�Ƃ�z�肵�Ă���
*/
typedef std::function<void(CmdListsArg_t, RTResourcesArg_t, RTDescHeapArg_t)> InitFunctionObject_t;
typedef std::function<void(CmdListsArg_t, RTResourcesArg_t)> LastFunctionObject_t;

/**
*�����_�����O�p�X�����ԂɌĂ΂�邱�Ƃ�ۏ؂���
*���ׂẴp�X���I��������R�}���h���X�g�����Z�b�g����
*/
class RenderingPathManager
{
public:
	~RenderingPathManager();

	struct RenderTargetViewObject
	{
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resource;
		std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> cpuhandles;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescHeap;//rendertargets��Descheap
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
	*RenderingManager�̏���������
	*/
	void Init(Microsoft::WRL::ComPtr<ID3D12Device>& dev, Microsoft::WRL::ComPtr<IDXGIFactory4>& factory, HWND hwnd);

	void Render();

	/**
	*�p�X�ԍ���0����
	*�p�X�̃C���f�b�N�X���炻�̃p�X�̃R�}���h��ςނׂ��R�}���h���X�g���擾����
	*DeleteRenderingPath���\�b�h���Ă񂾌ゾ�ƃC���f�b�N�X������邪�R�}���h���X�g���͕̂ω����Ȃ�
	*/
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetRenderingPathCommandList(unsigned int pathIndex) const;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetRenderingPathCommandList(unsigned int pathIndex, unsigned int cmdListIndex) const;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetRenderingPathCommandList(unsigned int pathIndex, const std::string& cmdListName) const;

	/**
	*�p�X�̖��O���炻�̃p�X�̃R�}���h��ςނׂ��R�}���h���X�g���擾����
	*DeleteRenderingPath���\�b�h���Ă񂾌ゾ�ƃC���f�b�N�X������邪�R�}���h���X�g���͕̂ω����Ȃ�
	*/
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetRenderingPathCommandList(const std::string& pathName) const;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetRenderingPathCommandList(const std::string& pathName, unsigned int cmdListIndex) const;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetRenderingPathCommandList(const std::string& pathName, const std::string& cmdListName) const;

	/**
	*�p�X�̖��O���炻�̃p�X�̃C���f�b�N�X���擾����
	*�����_�����O�p�X�̃C���f�b�N�X���擾����
	*DeleteRenderingPath���\�b�h���Ă񂾌ゾ�ƃC���f�b�N�X�������̂Œ���
	*/
	unsigned int GetRenderingPathIndex(const std::string& pathName) const;

	/**
	*�����_�[�^�[�Q�b�g��CPUHANDLE,DESCRIPTOR_HEAP,ID3D12Resource�������Ă���\���̂̎Q�Ƃ�Ԃ�
	*CPUHANDLE��ID3D12Resrouce�̏��Ԃ͑Ή����Ă���
	*DeleteRenderingPath���\�b�h���Ă񂾌ゾ�ƃC���f�b�N�X�������̂Œ���
	*/
	const RenderTargetViewObject& GetRenderTargetViews(unsigned int pathIndex) const;

	void CreateDummyRenderTarget(Microsoft::WRL::ComPtr<ID3D12Resource>& resource);

	/**
	*���ŃR�}���h���X�g����
	*�����_�����O�p�X�̖��O�����
	*�����ɕԂ�l�p�̃p�X������
	*�����_�����O�p�X�����������ƒǉ�����Ȃ�
	*�ǉ��ł��Ȃ��Ƃ�out_PathIndex��UINT_MAX��Ԃ�
	*initFunc�ɖ��t���[���̏��������ɂ��Ăق�����������ꂽ�����_����o�^���邱�Ƃ�
	*���t���[���Ăяo����邱�Ƃ�ۏ؂���
	*initFunc�̎��s�̌�Ƀ����_�[�^�[�Q�b�g���Z�b�g����
	*�O�ŕʂ̎g�����Ő�������ID3D12Resource��n���Ƃ���ɑΉ�����RenderTargetView�����
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
	*�w�肵�������_�����O�p�X�̃R�}���h���X�g���p�X�ォ��폜����
	*��������s������̓p�X�̃C���f�b�N�X�������̂Œ���
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
	*�p�X�̑S�̂̐����擾
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

