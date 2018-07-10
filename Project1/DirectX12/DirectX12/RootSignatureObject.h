#pragma once
#include <vector>
#include <d3dcommon.h>
#include <wrl.h>
#include <d3d12.h>

struct ID3D12RootSignature;
struct ID3D10Blob;

class RootSignatureObject
{
public:
	RootSignatureObject(ID3D10Blob* signatureBlob, Microsoft::WRL::ComPtr<ID3D12Device> dev);
	~RootSignatureObject();

	Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSignature();
private:
	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature;
};

