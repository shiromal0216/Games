/*
	ExtendLibMath.cpp
	�v�Z���s���ۂɕ֗��ƂȂ�@�\����������N���X�B
	���̋@�\���쐬�����͍̂��ؐ搶�B

	�쐬�ҁ@�F�@�x��@�a��
	�쐬���@�F�@2021/03/16
*/

#include <float.h>
#include <math.h>
#include <DxLib.h>
#include "ExtendLibMath.h"


ExtendLibMath::ExtendLibMath()
{

}

ExtendLibMath::~ExtendLibMath()
{

}



//----------------------------------------------------------------------
//! @brief �p�x�̒P�ʕϊ�([�x]��[���W�A��])
//!
//! @param[in] degree �p�x[�x]
//!
//! @return �p�x[���W�A��]
//----------------------------------------------------------------------
float ExtendLibMath::ToRadians(float degree)
{
	return degree * DX_PI_F / 180.0f;
}



//----------------------------------------------------------------------
//! @brief �p�x�̒P�ʕϊ�([���W�A��]��[�x])
//!
//! @param[in] radian �p�x[���W�A��]
//!
//! @return �p�x[�x]
//----------------------------------------------------------------------
float ExtendLibMath::ToDegrees(float radian)
{
	return radian * 180.0f / DX_PI_F;
}



//----------------------------------------------------------------------
//! @brief �w�肵���l�������`����͈͓̔��Ɏ��߂�(int��)
//!
//! @param[in] x   �͈͓��Ɏ��߂����l�̌��ݒl
//! @param[in] min ����
//! @param[in] max ���
//!
//! @return �����`����͈͓̔��Ɏ��߂��l
//----------------------------------------------------------------------
int ExtendLibMath::ClampI(int x, int min, int max)
{
	if (x < min)
	{
		x = min;
	}
	else if (x > max)
	{
		x = max;
	}

	return x;
}



//----------------------------------------------------------------------
//! @brief �w�肵���l���A�����`����͈͓̔��Ɏ��߂�(float��)
//!
//! @param[in] x   �͈͓��Ɏ��߂����l�̌��ݒl 
//! @param[in] min ����
//! @param[in] max ���
//!
//! @return �����`����͈͓̔��Ɏ��߂��l
//----------------------------------------------------------------------
float ExtendLibMath::ClampF(float x, float min, float max)
{
	if (x < min)
	{
		x = min;
	}
	else if (x > max)
	{
		x = max;
	}

	return x;
}



//----------------------------------------------------------------------
//! @brief ���������_���^�̌덷���l���ɓ��ꂽ��r
//!
//! @param[in] a ��r�Ώۂ̒l
//! @param[in] b ��r�Ώۂ̒l
//!
//! @retval TRUE  �덷���w��͈�(FLT_EPSILON)�ȓ��̏ꍇ
//! @retval FALSE ����ȊO�̏ꍇ
//----------------------------------------------------------------------
BOOL ExtendLibMath::FloatEquals(float a, float b)
{
	if (fabsf(a - b) < FLT_EPSILON)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}




