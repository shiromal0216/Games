/*
	ExtendLibMath.h
	�v�Z���s���ۂɕ֗��ƂȂ�@�\����������N���X�B

	�쐬�ҁ@�F�@�x��@�a��
	�쐬���@�F�@2021/03/16
*/
#pragma once
#include <minwindef.h>


class ExtendLibMath
{
private:


public:
	ExtendLibMath();
	~ExtendLibMath();

	// [�x]����[���W�A��]�ւ̒P�ʕϊ�
	float ToRadians(float degree);
	// [���W�A��]����[�x]�ւ̒P�ʕϊ�
	float ToDegrees(float radian);
	// �w�肵���l�������`����͈͓̔��Ɏ��߂�(int��)
	int   ClampI(int   x, int   min, int   max);
	// �w�肵���l�������`����͈͓̔��Ɏ��߂�(float��)
	float ClampF(float x, float min, float max);
	// ���������_���^�̌덷���l���ɓ��ꂽ��r
	BOOL FloatEquals(float a, float b);

};
