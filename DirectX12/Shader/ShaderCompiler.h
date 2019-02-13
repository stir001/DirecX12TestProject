#pragma once
#include "ShaderDatasStructure.h"

#include <string>
#include <map>
#include <vector>
#include <list>

class ShaderCompiler
{
public:
	~ShaderCompiler();

	static ShaderCompiler& Instance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new ShaderCompiler();
		}

		return *mInstance;
	}

	static void Destroy()
	{
		if (mInstance != nullptr)
		{
			delete mInstance;
		}
		mInstance = nullptr;
	}

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

	void AddDefineMacro(const std::string& name, const std::string& def);

	void SetShaderModel(const std::string& shaderModel);

	void SetShaderDirPath(std::string shaderDirPath);

	const std::string& GetShaderDirPath() const;

private:
	ShaderCompiler();
	ShaderCompiler(const ShaderCompiler&);
	ShaderCompiler(const ShaderCompiler&&);
	ShaderCompiler& operator=(const ShaderCompiler&);

	static ShaderCompiler* mInstance;
	struct MacroData
	{
		std::string name;
		std::string def;
	};

	std::map<std::string, ShaderDatas> mDatas;
	std::vector<D3D_SHADER_MACRO> mMacros;
	std::list<MacroData> mStrData;
	std::string mShaderModel;
	std::string mShaderDirPath;
};
