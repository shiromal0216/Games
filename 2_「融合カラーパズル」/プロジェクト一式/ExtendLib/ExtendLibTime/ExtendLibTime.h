/*
	ExtendLibTime.h
	�Q�[�����Ŏ��ԂɊւ��鏈�����ȗ������邽�߂̃N���X�B

	�쐬�ҁ@�F�@�x��@�a��
	�쐬���@�F�@2021/03/16
*/
#pragma once
#include <windows.h>

class ExtendLibTime
{
private:
	static const int ONE_SECOND;

	float s_frameDeltaTime;					//	 �t���[���̌o�ߎ���[s]
	float s_frameRate;						//	 �t���[�����[�g[fps]

	LONGLONG previousTime;                  //	 �O��̌v������
	LONGLONG currentTime;					//	 ���݂̌v������
	LONGLONG startTime;						//	 �t���[�����[�g�v���J�n����
	int      frameCount;					//	 �t���[����


public:
	ExtendLibTime();
	~ExtendLibTime();

	//	�t���[���^�C�}�[�̍X�V
	void  UpdateFrameTimer();

	//	���O�̃t���[������̌o�ߎ��Ԃ̎擾
	float GetFrameDeltaTime();

	//	�t���[�����[�g�̎擾
	float GetFrameRate();
};

