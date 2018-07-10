#include "Image3DController.h"
#include "ImageObject.h"
#include "VertexBufferObject.h"
#include "ConstantBufferObject.h"
#include "Rect.h"
#include "XMFloatOperators.h"
#include "RenderingPathManager.h"
#include "PipelineStateObject.h"
#include "RootSignatureObject.h"
#include "Dx12Ctrl.h"
#include "DepthBufferObject.h"
#include "Dx12Camera.h"
#include "Dx12DescriptorHeapObject.h"
#include "ShaderResourceObject.h"
#include "Dx12CommandList.h"

const unsigned int DEFAULT_RESOURCE_NUM = 3;

Image3DController::Image3DController(std::shared_ptr<ImageObject> img, 
	const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList,
	std::shared_ptr<PipelineStateObject>& pipelinestate,
	std::shared_ptr<RootSignatureObject>& rootsignature)
	:DrawObjectController(img->GetTextureName() + "Bundle",dev, cmdList),
	mImgObj(img)
	, mVertex{ { { 0.f, img->GetImageSize().y, 0.f ,1.f}, {0.f,0.f,-1.f,0.f}, { 0.f, 0.f } }/* v1 */,
{ { img->GetImageSize().x,img->GetImageSize().y, 0.f ,1.f}, { 0.f,0.f,-1.f,0.f }, { 1.f, 0.f } }/* v2 */
,{ { 0.0f, 0.0f, 0.0f,1.f },{ 0.f,0.f,-1.f,0.f },{ 0.f, 1.f } }/* v3 */
,{ { img->GetImageSize().x, 0.0f, 0.f, 1.f },{ 0.f,0.f,-1.f,0.f }, { 1.f, 1.f } }/* v4 */ }
, mScaleX(1.0f), mScaleY(1.0f), mScaleZ(1.0f), mCenter({ 0.f,0.f,0.f })
, mRect(new Rect(mCenter, img->GetImageSize().x, img->GetImageSize().y))
, mTurnSign(1, 1),mBundleUpdate(&Image3DController::UpdateBundle)
{
	mPipelinestate = pipelinestate;
	mRootsignature = rootsignature;

	mCameraBuffer = Dx12Ctrl::Instance()->GetCamera()->GetCameraBuffer();
	std::string name = mImgObj->GetTextureName();

	name += "3DImageVertexBuffer";
	mVertexBuffer.reset(new VertexBufferObject(name, Dx12Ctrl::Instance()->GetDev(), sizeof(Image3DVertex), 4));

	name = mImgObj->GetTextureName();
	name += "3DImageMatrixConstantBuffer";
	mImageMatrixBuffer.reset(new ConstantBufferObject(name, Dx12Ctrl::Instance()->GetDev(), sizeof(DirectX::XMFLOAT4X4), 1));

	std::vector<std::shared_ptr<Dx12BufferObject>> resources;
	resources.reserve(DEFAULT_RESOURCE_NUM);
	resources.push_back(mCameraBuffer);
	resources.push_back(mImageMatrixBuffer);
	resources.push_back(mImgObj->GetShaderResource());
	name = mImgObj->GetTextureName();
	name += "3DImageDescriptorHeap";
	mDescHeap.reset(new Dx12DescriptorHeapObject(name, Dx12Ctrl::Instance()->GetDev(), resources, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

	DirectX::XMStoreFloat4x4(&mImageMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&mRotaMatrix, DirectX::XMMatrixIdentity());
	UpdateMatrix();
	UpdateBuffer();
	mBundleCmdList->Close();
}

Image3DController::~Image3DController()
{
	delete mRect;
}

void Image3DController::AddCenterPos(const float x, const float y, const float z)
{
	mCenter.x += x;
	mCenter.y += y;
	mCenter.z += z;
	UpdateMatrix();
	UpdateBuffer();
}

void Image3DController::AddCenterPos(const DirectX::XMFLOAT3& offset)
{
	AddCenterPos(offset.x, offset.y, offset.z);
}

void Image3DController::AddRotaX(float deg)
{
	DirectX::XMStoreFloat4x4(&mRotaMatrix, DirectX::XMLoadFloat4x4(&mRotaMatrix) * DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(deg)));
	UpdateMatrix();
	UpdateBuffer();
}

void Image3DController::AddRotaY(float deg)
{
	DirectX::XMStoreFloat4x4(&mRotaMatrix, DirectX::XMLoadFloat4x4(&mRotaMatrix) * DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(deg)));
	UpdateMatrix();
	UpdateBuffer();
}

void Image3DController::AddRotaZ(float deg)
{
	DirectX::XMStoreFloat4x4(&mRotaMatrix, DirectX::XMLoadFloat4x4(&mRotaMatrix) * DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(deg)));
	UpdateMatrix();
	UpdateBuffer();
}

void Image3DController::AddScale(const float scale)
{
	AddScale(scale, scale, scale);
}

void Image3DController::AddScale(const float scaleX, const float scaleY, const float scaleZ)
{
	mScaleX += scaleX;
	mScaleY += scaleY;
	mScaleZ += scaleZ;
	UpdateMatrix();
	UpdateBuffer();
}

void Image3DController::SetRect(const DirectX::XMFLOAT3& inc, const float inw, const float inh)
{
	mRect->SetCenter(inc);
	mRect->SetHeight(inh);
	mRect->SetWidth(inw);

	UpdateUV();
	UpdateBuffer();
}

void Image3DController::SetRect(const Rect& rc)
{
	SetRect(rc.GetCenter(), rc.GetWidth(), rc.GetHeight());
}

void Image3DController::SetCenterPos(const float x, const float y, const float z)
{
	mCenter.x = x;
	mCenter.y = y;
	mCenter.z = z;

	UpdateMatrix();
	UpdateBuffer();
}

void Image3DController::SetCenterPos(const DirectX::XMFLOAT3& setPos)
{
	SetCenterPos(setPos.x, setPos.y, setPos.z);
}

void Image3DController::SetScale(const float s)
{
	SetScale(s, s, s);
}

void Image3DController::SetScale(const float sx, const float sy, const float sz)
{
	mScaleX = sx;
	mScaleY = sy;
	mScaleZ = sz;
	UpdateMatrix();
	UpdateBuffer();
}

void Image3DController::TurnX()
{
	DirectX::XMFLOAT2 uv;
	uv = mVertex[0].uv;
	mVertex[0].uv = mVertex[1].uv;
	mVertex[1].uv = uv;

	uv = mVertex[2].uv;
	mVertex[2].uv = mVertex[3].uv;
	mVertex[3].uv = uv;

	mTurnSign.x *= -1;

	UpdateMatrix();
	UpdateBuffer();
}

void Image3DController::TurnY()
{
	DirectX::XMFLOAT2 uv;
	uv = mVertex[0].uv;
	mVertex[0].uv = mVertex[2].uv;
	mVertex[2].uv = uv;

	uv = mVertex[1].uv;
	mVertex[1].uv = mVertex[3].uv;
	mVertex[3].uv = uv;

	mTurnSign.y *= -1;

	UpdateMatrix();
	UpdateBuffer();
}

void Image3DController::Draw()
{
	DX12CTRL_INSTANCE;
	//auto obj = RenderingPathManager::Instance()->GetRenderTargetViews(0);
	//mCmdList->OMSetRenderTargets(obj.cpuhandles.size(), &obj.rtvDescHeap->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(), false, &Dx12Ctrl::Instance()->GetDepthBuffer()->GetCPUAdress());
	(this->*mBundleUpdate)();
	mDescHeap->SetDescriptorHeap(mCmdList);
	mCmdList->ExecuteBundle(mBundleCmdList->GetCommandList().Get());
}

DirectX::XMFLOAT2 Image3DController::GetImageSize()
{
	return mImgObj->GetImageSize();
}

std::shared_ptr<Image3DController> Image3DController::GetNewCopy()
{
	std::shared_ptr<Image3DController> rtn(new Image3DController(mImgObj,Dx12Ctrl::Instance()->GetDev(), mCmdList, mPipelinestate, mRootsignature));
	return rtn;
}

void Image3DController::SetRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature)
{
	mRootsignature = rootsignature;
	mBundleUpdate = &Image3DController::UpdateBundle;
}

void Image3DController::SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate)
{
	mPipelinestate = pipelinestate;
	mBundleUpdate = &Image3DController::UpdateBundle;
}

void Image3DController::SetCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
{
	mCmdList = cmdList;
}

std::string Image3DController::GetImageName() const
{
	return mImgObj->GetTextureName();
}

std::string Image3DController::GetFilePath() const
{
	return mImgObj->GetFilePath();
}

void Image3DController::UpdateUV()
{
	DirectX::XMFLOAT2 size = mImgObj->GetImageSize();
	DirectX::XMFLOAT2 leftupUV = { mRect->GetLeft() / size.x,mRect->GetDown() / size.y };
	DirectX::XMFLOAT2 rightdownUV = { mRect->GetRight() / size.x, mRect->GetUp() / size.y };
	mVertex[0].uv = leftupUV;

	mVertex[1].uv.x = rightdownUV.x;
	mVertex[1].uv.y = leftupUV.y;

	mVertex[2].uv.x = leftupUV.x;
	mVertex[2].uv.y = rightdownUV.y;

	mVertex[3].uv = rightdownUV;

	if (mTurnSign.x == -1)
	{
		DirectX::XMFLOAT2 uv;
		uv = mVertex[0].uv;
		mVertex[0].uv = mVertex[1].uv;
		mVertex[1].uv = uv;

		uv = mVertex[2].uv;
		mVertex[2].uv = mVertex[3].uv;
		mVertex[3].uv = uv;
	}
	if (mTurnSign.y == -1)
	{
		DirectX::XMFLOAT2 uv;
		uv = mVertex[0].uv;
		mVertex[0].uv = mVertex[2].uv;
		mVertex[2].uv = uv;

		uv = mVertex[1].uv;
		mVertex[1].uv = mVertex[3].uv;
		mVertex[3].uv = uv;
	}
}

void Image3DController::UpdateMatrix()
{
	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(mScaleX, mScaleY, mScaleZ);
	DirectX::XMMATRIX tranlationMatrix = DirectX::XMMatrixTranslation(mCenter.x, mCenter.y, mCenter.z);

	DirectX::XMMATRIX imageMatrix = DirectX::XMMatrixIdentity() * DirectX::XMLoadFloat4x4(&mRotaMatrix) * scaleMatrix * tranlationMatrix;
	DirectX::XMStoreFloat4x4(&mImageMatrix, imageMatrix);
}

void Image3DController::UpdateBuffer()
{
	mVertexBuffer->WriteBuffer(mVertex, sizeof(Image3DVertex) * 4);
	mImageMatrixBuffer->WriteBuffer256Alignment(&mImageMatrix, sizeof(DirectX::XMFLOAT4X4), 1);
}

void Image3DController::UpdateBundle()
{
	mBundleCmdList->Reset();
	const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& bundle = mBundleCmdList->GetCommandList();

	bundle->SetPipelineState(mPipelinestate->GetPipelineState().Get());
	bundle->SetGraphicsRootSignature(mRootsignature->GetRootSignature().Get());
	bundle->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	mVertexBuffer->SetBuffer(bundle);
	mDescHeap->SetDescriptorHeap(bundle);

	unsigned int resourceIndex = 0;

	mDescHeap->SetGprahicsDescriptorTable(bundle, resourceIndex++, Image3DController::eROOT_PARAMATER_INDEX_CAMERA);
	mDescHeap->SetGprahicsDescriptorTable(bundle, resourceIndex++, Image3DController::eROOT_PARAMATER_INDEX_MATRIX);
	mDescHeap->SetGprahicsDescriptorTable(bundle, resourceIndex++, Image3DController::eROOT_PARAMATER_INDEX_TEXTURE);
	bundle->DrawInstanced(4, 1, 0, 0);

	bundle->Close();
	mBundleUpdate = &Image3DController::NonUpdateBundle;
}

void Image3DController::NonUpdateBundle()
{
}
