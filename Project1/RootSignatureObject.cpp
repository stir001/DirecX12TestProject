#include "RootSignatureObject.h"
#include "Dx12Ctrl.h"
#include "DescriptorHeapManager.h"
#include <d3d12.h>
#include "d3dx12.h"


RootSignatureObject::RootSignatureObject() :srvRangecount(0), cbvRangecount(0),signature(nullptr),error(nullptr)
{
	//先にdescriptorRangeを定義
	descriptorRanges.push_back(MakeDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, srvRangecount++));
	descriptorRanges.push_back(MakeDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, cbvRangecount++));
	descriptorRanges.push_back(MakeDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, cbvRangecount++));
	descriptorRanges.push_back(MakeDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, cbvRangecount++));//カメラ用
	descriptorRanges.push_back(MakeDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, srvRangecount++));//シャドウマップ用
	
	//descriptorRangeの順番にrootparamを定義
	rootparam.push_back(MakeRootParamater(&descriptorRanges[rpt_texture] , D3D12_SHADER_VISIBILITY_PIXEL));
	rootparam.push_back(MakeRootParamater(&descriptorRanges[rpt_cbv] , D3D12_SHADER_VISIBILITY_ALL));
	rootparam.push_back(MakeRootParamater(&descriptorRanges[rpt_bonematrix] , D3D12_SHADER_VISIBILITY_ALL));
	rootparam.push_back(MakeRootParamater(&descriptorRanges[rpt_camera] , D3D12_SHADER_VISIBILITY_ALL));
	rootparam.push_back(MakeRootParamater(&descriptorRanges[rpt_shadow] , D3D12_SHADER_VISIBILITY_PIXEL));

	
	//中でsamplerを定義
	CreateRootSignature();
}

RootSignatureObject::RootSignatureObject(ID3D10Blob* signatureBlob)
{
	DX12CTRL_INSTANCE
	d12->result =d12->GetDev()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
}

RootSignatureObject::~RootSignatureObject()
{
	rootSignature->Release();
	//signature->Release();
	//error->Release();
	delete rootSignatureDesc;
}

D3D12_DESCRIPTOR_RANGE RootSignatureObject::MakeDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE type,int numragister, int numDescriptor)
{
	D3D12_DESCRIPTOR_RANGE range;
	range.RangeType = type;
	range.BaseShaderRegister = numragister;
	range.NumDescriptors = numDescriptor;
	range.RegisterSpace = 0;
	range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	return range;
}

D3D12_ROOT_PARAMETER RootSignatureObject::MakeRootParamater(D3D12_DESCRIPTOR_RANGE* range, D3D12_SHADER_VISIBILITY visiblity, D3D12_ROOT_PARAMETER_TYPE type , int numDescriptorRange)
{
	D3D12_ROOT_PARAMETER rtn;
	ZeroMemory(&rtn,sizeof(rtn));
	rtn.DescriptorTable.NumDescriptorRanges = numDescriptorRange;
	rtn.DescriptorTable.pDescriptorRanges = range;
	rtn.ShaderVisibility = visiblity;
	rtn.ParameterType = type;
	return rtn;
}

int RootSignatureObject::AddDescriptorRange(D3D12_DESCRIPTOR_RANGE& inrange)
{
	descriptorRanges.push_back(inrange);
	return static_cast<int>(descriptorRanges.size() - 1);
}

int RootSignatureObject::AddDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE type ,int numDescriptor, int* outNum)
{
	int registerNum = 0;
	switch (type)
	{
	case D3D12_DESCRIPTOR_RANGE_TYPE_SRV:
		registerNum = srvRangecount++;
		break;
	case D3D12_DESCRIPTOR_RANGE_TYPE_UAV:
		registerNum = uavRangecount++;
		break;
	case D3D12_DESCRIPTOR_RANGE_TYPE_CBV:
		registerNum = cbvRangecount++;
		break;
	case D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER:
		registerNum = smpRangecount++;
		break;
	default:
		break;
	}
	if (outNum != nullptr) *outNum = registerNum;

	D3D12_DESCRIPTOR_RANGE descRange = D3D12_DESCRIPTOR_RANGE();
	descRange.RangeType = type;
	descRange.BaseShaderRegister = registerNum;
	descRange.NumDescriptors = numDescriptor;
	descRange.RegisterSpace = 0;
	descRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	descriptorRanges.push_back(descRange);
	return static_cast<int>( descriptorRanges.size() - 1);
}

void RootSignatureObject::AddRootParam(D3D12_ROOT_PARAMETER& inrparam)
{
	rootparam.push_back(inrparam);
}

void RootSignatureObject::CreateRootSignature()
{
	/*for (int i = 0; i < descriptorRanges.size(); i++)
	{
		D3D12_SHADER_VISIBILITY visibility;
		switch (descriptorRanges[i].RangeType)
		{
		case D3D12_DESCRIPTOR_RANGE_TYPE_SRV:
			visibility = D3D12_SHADER_VISIBILITY_PIXEL;
			break;
		case D3D12_DESCRIPTOR_RANGE_TYPE_UAV:
			visibility = D3D12_SHADER_VISIBILITY_ALL;
			break;
		case D3D12_DESCRIPTOR_RANGE_TYPE_CBV:
			visibility = D3D12_SHADER_VISIBILITY_ALL;
			break;
		case D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER:
			visibility = D3D12_SHADER_VISIBILITY_PIXEL;
			break;
		default:
			visibility = D3D12_SHADER_VISIBILITY_ALL;
			break;
		}

		rootparam.push_back(MakeRootParamater(&descriptorRanges[i], visibility));
	}*/
	Dx12Ctrl* d12 = Dx12Ctrl::Instance();
	rootSignatureDesc = new CD3DX12_ROOT_SIGNATURE_DESC();
	rootSignatureDesc->Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	CreateSamplers();
	rootSignatureDesc->pParameters = &rootparam[0];
	rootSignatureDesc->NumParameters = static_cast<UINT>(rootparam.size());
	d12->result = D3D12SerializeRootSignature(rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
	D12RESULTCHECK
	d12->result = d12->GetDev()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	D12RESULTCHECK
}

void RootSignatureObject::CreateSamplers()
{
	const int samplercount= 1;
	samplerDesc.resize(samplercount);
	ZeroMemory(&samplerDesc[0], sizeof(samplerDesc[0]) * samplercount);

	samplerDesc[0] = Dx12Ctrl::Instance()->GetDefaulSamplerDesc();
	
	rootSignatureDesc->NumStaticSamplers = static_cast<UINT>(samplerDesc.size());
	rootSignatureDesc->pStaticSamplers = &samplerDesc[0];
}

Microsoft::WRL::ComPtr<ID3D12RootSignature> RootSignatureObject::GetRootSignature()
{
	return rootSignature;
}

void RootSignatureObject::Release()
{
	rootSignature->Release();
	signature->Release();
	error->Release();
	delete(rootSignatureDesc);
}