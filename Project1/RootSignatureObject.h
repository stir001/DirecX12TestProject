#pragma once
#include <vector>
#include <d3dcommon.h>
#include<wrl.h>

struct ID3D12RootSignature;
struct ID3D10Blob;
struct D3D12_DESCRIPTOR_RANGE;
struct D3D12_ROOT_PARAMETER;
struct CD3DX12_ROOT_SIGNATURE_DESC;
struct D3D12_STATIC_SAMPLER_DESC;
enum D3D12_DESCRIPTOR_RANGE_TYPE;
enum D3D12_ROOT_PARAMETER_TYPE;
enum D3D12_SHADER_VISIBILITY;

enum RootParamType
{
	rpt_texture,	//srv
	rpt_camera,		//カメラ
	rpt_light,		//ライト
	rpt_cbv,		//cbv マテリアル
	rpt_bonematrix,	//回転行列
	rpt_shadow,
	rpt_max
};

class RootSignatureObject
{
private:
	int srvRangecount;//レジスタt
	int cbvRangecount;//レジスタb
	int uavRangecount;//レジスタu
	int smpRangecount;//レジスタs
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	ID3D10Blob* signature;
	ID3D10Blob* error;
	std::vector<D3D12_DESCRIPTOR_RANGE> descriptorRanges;
	std::vector<D3D12_ROOT_PARAMETER> rootparam;
	CD3DX12_ROOT_SIGNATURE_DESC* rootSignatureDesc;
	std::vector<D3D12_STATIC_SAMPLER_DESC> samplerDesc;
	void CreateSamplers();
	D3D12_DESCRIPTOR_RANGE MakeDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE type, int numragister, int numDescriptor = 1);
	D3D12_ROOT_PARAMETER MakeRootParamater(D3D12_DESCRIPTOR_RANGE* range,  D3D12_SHADER_VISIBILITY visiblity, D3D12_ROOT_PARAMETER_TYPE type = static_cast<D3D12_ROOT_PARAMETER_TYPE>(0), int numDescriptorRange = 1);
	
	void AddRootParam(D3D12_ROOT_PARAMETER& inrparam);
public:
	//@return rootParamaterIndex
	int AddDescriptorRange(D3D12_DESCRIPTOR_RANGE& inrange);

	//@param registerNum _out_ 割り当てレジスタ番号が入る確認用 デフォルトでnullptr
	//@return rootParamaterIndex
	int AddDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE type, int numDescriptor = 1, int* registerNum = nullptr);
	void CreateRootSignature();


	Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSignature();
	void Release();
	RootSignatureObject();
	RootSignatureObject(ID3D10Blob* signatureBlob);
	~RootSignatureObject();
};

