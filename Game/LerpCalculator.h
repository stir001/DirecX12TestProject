#pragma once
#include <Dx12MSLibCommon.h>
#include <cmath>

class LerpCalculator
{
public:
	LerpCalculator();
	~LerpCalculator();

	/**
	*	@brief	���`��Ԃ����l���v�Z����
	*	@param[in]	first	�ŏ��̒l
	*	@param[in]	end		�ŏI�̒l
	*	@param[in]	t		0.0f~1.0f�̊Ԃ̒l
	*	@return	�v�Z��̒l
	*/
	template <typename T>
	T Lerp(const T& first, const T& end, const float t)
	{
		float scaler = 0.0f;
		if (t < 0.0f)
		{
			scaler = 0.0f;
		}
		else if (t > 1.0f)
		{
			scaler = 1.0f;
		}
		else
		{
			scaler = t;
		}
		return first * (1.0f - scaler) + end * scaler;
	}

	/**
	*	@brief	���ʐ��`��Ԃ����l���v�Z����
	*	@param[in]	first	�ŏ��̒l
	*	@param[in]	end		�ŏI�̒l
	*	@param[in]	t		0.0f~1.0f�̊Ԃ̒l
	*	@return	�v�Z��̒l
	*/
	template <typename T>
	T Slerp(const T& first, const T& end, const float t)
	{
		float scaler = 0.0f;
		if (t < 0.0f)
		{
			scaler = 0.0f;
		}
		else if (t > 1.0f)
		{
			scaler = 1.0f;
		}
		else
		{
			scaler = t;
		}

		float pi_div2 = 3.14159265f / 2.0f;
		float sin = std::sinf((1.0f - scaler) * pi_div2);
		float cos = std::cosf(scaler * pi_div2);

		return first * sin * sin + end * cos * cos;
	}
};

