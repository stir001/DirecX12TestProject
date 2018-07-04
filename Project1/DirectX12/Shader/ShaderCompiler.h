#pragma once
#include "ShaderDatasStructure.h"

#include <string>
#include <map>

class ShaderCompiler
{
public:
	~ShaderCompiler();

	/**@fn
	*�n���ꂽ�V�F�[�_�[�̃p�X�Ɩ��O����V�F�[�^�[���R���p�C������
	*rootSignature��hlsl�ɒ�`����Ă���Ƃ��͒��_�V�F�[�_�[�Ɋ֘A�t���Ă�������
	*�����Ȃ��Ƃ��͋�̈�����n������
	*@brief hlsl�̃R���p�C�������ăf�[�^��ۑ�����
	*@param (shaderPath) �V�F�[�_�̃t�@�C���p�X
	*@param (vsName) �o�[�e�b�N�X�V�F�[�_�[��
	*@param (psName) �s�N�Z���V�F�[�_�[�̖�
	*@param (gsName) �W�I���g���V�F�[�_�[��
	*@param (hsName) �n���V�F�[�_�[��
	*@param (dsname) �h���C���V�F�[�_�[��
	*@param (existRootSignature) ���[�g�V�O�l�`�����`���Ă��邩�ǂ���
	*/
	ShaderDatas CompileShader(const std::string& shaderPath,
		const std::string& vsName, 
		const std::string& psName, 
		const std::string& gsName, 
		const std::string& hsName,
		const std::string& dsName,
		bool existRootSignature);

	void ReleaseShader(std::string shaderpath);
	static ShaderCompiler* GetInstance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new ShaderCompiler();
		}

		return mInstance;
	}

	static void Destroy()
	{
		if (mInstance != nullptr)
		{
			delete mInstance;
		}
		mInstance = nullptr;
	}
private:
	ShaderCompiler();
	ShaderCompiler(const ShaderCompiler&);
	ShaderCompiler(const ShaderCompiler&&);
	ShaderCompiler& operator=(const ShaderCompiler&);

	static ShaderCompiler* mInstance;

	std::map<std::string, ShaderDatas> mDatas;

};
