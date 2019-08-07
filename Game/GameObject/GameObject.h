#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>

class PrimitiveController;
class PrimitiveObject;
struct PrimitiveVertex;

class GameObject
{
public:
	GameObject();
	GameObject(const std::shared_ptr<PrimitiveController>& ctrl, const std::shared_ptr<PrimitiveObject>& data);
	~GameObject();

	/**
	*	@brief	��Ԃ��X�V����
	*/
	void Update();

	/**
	*	@brief	���x��ݒ肷��
	*	@param[in]	vel	�ݒ肷�鑬�x
	*/
	void SetVelocity(const DirectX::XMFLOAT3& vel);

	/**
	*	@brief	���x���擾����
	*	@return	���݂̑��x
	*/
	DirectX::XMFLOAT3 GetVelocity() const;

	/**
	*	@brief	���W��ݒ肷��
	*	@param[in]	pos	�ݒ肷����W
	*/
	void SetPos(const DirectX::XMFLOAT3& pos);

	/**
	*	@brief	���W���擾����
	*	@return	���݂̍��W
	*/
	DirectX::XMFLOAT3 GetPos() const;

	/**
	*	@brief	�`�悷��
	*/
	void Draw() const;

	/**
	*	@brief	PrimitiveObject���擾����
	*/
	std::shared_ptr<PrimitiveObject>& GetData();

	/**
	*	@brief	���_�����擾����(���ڂ̕ύX�s��)
	*	@return	���_���
	*/
	const std::vector<PrimitiveVertex>& GetVertices() const;

	/**
	*	@brief	�C���f�b�N�X�����擾����
	*	@return	�C���f�N�X���
	*/
	const std::vector<unsigned int>& GetIndices() const;

	/**
	*	@brief	PrimitiveController��PrimitiveObject���Đݒ肷��
	*	@param[in]	ctrl	�Đݒ肷��PrimitiveController
	*	@param[in]	data	�Đݒ肷��PrimitiveObject
	*/
	void Reset(const std::shared_ptr<PrimitiveController>& ctrl, const std::shared_ptr<PrimitiveObject>& data);

	/**
	*	@brief	�F��ݒ肷��
	*	@param[in]	�ݒ肷��F
	*/
	void SetColor(const DirectX::XMFLOAT3& color);

	/**
	*	�����蔻��p�̋��̔��a���擾����
	*/
	float GetCollisionRadius() const;
private:
	/**
	*	�����蔻��p���̔��a��ݒ肷��
	*/
	void SetCollisionRadius();

	std::shared_ptr<PrimitiveController> mCtrl;
	std::shared_ptr<PrimitiveObject> mData;

	DirectX::XMFLOAT3 mVel;
	DirectX::XMFLOAT3 mPos;

	/**
	*	��܂��ȓ����蔻�莞�ɗp����S�̂��͂މ~�̔��a
	*/
	float mCollisionRadius;
};

