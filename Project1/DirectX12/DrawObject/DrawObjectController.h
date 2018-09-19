#pragma once
/**
*	@addtogroup DrawObjectController
*	@file DrawObjectController.h
*	@brief �`��I�u�W�F�N�g�𑀍삷����N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/14
*/
#include <memory>
#include <wrl.h>
#include <d3d12.h>
#include <string>

class RootSignatureObject;
class PipelineStateObject;
class Dx12CommandList;

class DrawObjectController
{
public:
	DrawObjectController(const std::string& bundleName, const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
	virtual ~DrawObjectController();
	virtual void SetRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature);
	virtual void SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate);
	virtual void SetCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
protected:
	std::shared_ptr<RootSignatureObject> mRootsignature;
	std::shared_ptr<PipelineStateObject> mPipelinestate;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCmdList;
	std::shared_ptr<Dx12CommandList> mBundleCmdList;
private:
};

