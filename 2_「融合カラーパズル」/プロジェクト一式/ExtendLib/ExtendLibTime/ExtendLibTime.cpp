/*
	ExtendLibTime.cpp
	�Q�[�����Ŏ��ԂɊւ��鏈�����ȗ������邽�߂̃N���X�B
	���̋@�\���쐬�����͍̂��ؐ搶�B

	�쐬�ҁ@�F�@�x��@�a��
	�쐬���@�F�@2021/03/16
*/

#include <DxLib.h>
#include "ExtendLibTime.h"


const int ExtendLibTime::ONE_SECOND = 1000000;


ExtendLibTime::ExtendLibTime() :
	s_frameDeltaTime(0.0f),
	s_frameRate(0.0f),
	previousTime(0),
	currentTime(GetNowHiPerformanceCount()),
	startTime(0),
	frameCount(0)
{

}

ExtendLibTime::~ExtendLibTime()
{

}

// �t���[���^�C�}�[�̍X�V
void  ExtendLibTime::UpdateFrameTimer()
{
	// ���Ԃ̌v��
	previousTime = currentTime;
	currentTime = GetNowHiPerformanceCount();


	// �t���[���̌o�ߎ��Ԃ̍X�V
	s_frameDeltaTime = (float)(currentTime - previousTime) / ONE_SECOND;


	// 1�b���Ƀt���[�����[�g�̍X�V
	frameCount++;
	if ((currentTime - startTime) > ONE_SECOND)
	{
		s_frameRate = (float)(frameCount * ONE_SECOND) / (currentTime - startTime);
		startTime = currentTime;
		frameCount = 0;
	}
}

// ���O�̃t���[������̌o�ߎ��Ԃ̎擾
float ExtendLibTime::GetFrameDeltaTime()
{
	return s_frameDeltaTime;
}

// �t���[�����[�g�̎擾
float ExtendLibTime::GetFrameRate()
{
	return s_frameRate;
}
