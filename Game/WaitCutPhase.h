#pragma once
#include "Phase.h"
#include <memory>
#include <vector>
#include <DirectXMath.h>

class GameObject;
class PrimitiveCutter;
struct PrimitiveVertex;

/**
*	�ؒf���邽�߂̃t�F�[�Y
*/
class WaitCutPhase :
	public Phase
{
public:
	WaitCutPhase(PhaseChanger& changer,std::shared_ptr<GameObject>& baseObj);
	virtual ~WaitCutPhase();

	/**
	*	@brief	�Q�[���̍X�V
	*/
	void Update(const DxInput& input);

	/**
	*	@brief	�`��
	*/
	void Draw();

	/**
	*	@brief	����������
	*/
	void Initialize();

	/**
	*	@brief	�I������
	*/
	void Terminate();
private:
	/**
	*	@brief	�v���~�e�B�u��ؒf����
	*	@param[in]	origin	�ؒf����ۂ̕��ʂ̒ʂ�_
	*	@param[in]	normal	�ؒf���镽�ʂ̖@��
	*/
	void Cut(const DirectX::XMFLOAT3& origin, const DirectX::XMFLOAT3& normal);

	/**
	*	@brief	�S�Ă̒��_���W�𕽋ϒl�����_�A���S�Ƃ������W�ɍČv�Z����
	*	@param[in,out]	�Čv�Z���钸�_�Q
	*	@return			���_�Ƃ������ϒl
	*/
	DirectX::XMFLOAT3 RePlaceVertsPos(std::vector<PrimitiveVertex>& verts);

	/**
	*	@brief	�ؒf���邽�߂̌��_�Ɩ@�����擾����
	*	@return	���_�A�@��
	*/
	std::tuple<DirectX::XMFLOAT3, DirectX::XMFLOAT3> GetCutFace() const;

	std::shared_ptr<GameObject> mBase;
	std::shared_ptr<GameObject> mPlus;
	std::shared_ptr<GameObject> mMinus;
	std::unique_ptr<PrimitiveCutter> mCutter;
};

