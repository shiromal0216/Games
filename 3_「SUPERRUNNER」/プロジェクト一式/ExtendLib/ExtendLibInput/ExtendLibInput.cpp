/*
	ExtendLibInput.cpp
	DxLib�𗘗p���āA���͂̏�Ԃ��Ǘ�����N���X�B
	DxLib�ł́A�{�^����������Ă��邩������Ă��Ȃ����͕����邪�A
	�����ꂽ�u�Ԃ��A�����ꂽ�u�Ԃ��A�Ƃ����`�F�b�N���ł��Ȃ����߁A
	���̕������J�o�[���邽�߂̏������T�|�[�g���邽�߁A���ۂ̏������L�q����B


	�쐬�ҁ@�F�@�x��@�a��
	�쐬���@�F�@2021/03/15
*/
#include <DxLib.h>
#include "ExtendLibInput.h"

const int ExtendLibInput::KEY_CODE_MAX = 256;

//	�R���X�g���N�^
ExtendLibInput::ExtendLibInput()
{
	//	�ϐ��̏��������s��
	Initialize();
}

//	�f�X�g���N�^
ExtendLibInput::~ExtendLibInput()
{

}


//	����������
void ExtendLibInput::Initialize()
{
	//	�W���C�p�b�h�̃{�^�����p�ϐ��̏�����
	nowJoypadInput = 0;
	oldJoypadInput = 0;

	for (int i = 0; i < KEY_CODE_MAX; i++)
	{
		//	�L�[�{�[�h�̃{�^�����p�ϐ��̏�����
		nowKeyInput[i] = 0;
		oldKeyInput[i] = 0;
	}

}

//	�X�V����
void ExtendLibInput::Update()
{
	//	�ϐ�oldJoypadInput�ɕϐ�nowJoypadInput�����A1�t���[���O�̏����m�ۂ���
	oldJoypadInput = nowJoypadInput;

	//	���[�v��oldKeyInput[i]�̒���nowKeyInput[i]�����A1�t���[���O�̏����m�ۂ���
	for (int i = 0; i < KEY_CODE_MAX; i ++)
	{
		oldKeyInput[i] = nowKeyInput[i];
	}

	//	�W���C�p�b�h�̃{�^�������X�V����
	nowJoypadInput = GetJoypadInputState(DX_INPUT_PAD1);

	//	�L�[�{�[�h�̃{�^���������[�v�ōX�V����
	for (int i = 0; i < KEY_CODE_MAX; i++)
	{
		nowKeyInput[i] = CheckHitKey(i);
	}
}

//	�I������
void ExtendLibInput::Finalize()
{
	//	�����K�v�ȏI������������΁A�s��
}


//	�W���C�p�b�h�̃{�^����������Ă��邩�`�F�b�N
int ExtendLibInput::IsButtonDown(int buttonID)
{
	//	�w�肳�ꂽ�{�^��ID��������Ă����Ԃ��`�F�b�N����
	if ((nowJoypadInput & buttonID) != 0)
	{
		return 1;
	}

	return 0;
}

//	�W���C�p�b�h�̃{�^����������Ă��邩�`�F�b�N
int ExtendLibInput::IsButtonUp(int buttonID)
{
	//	�w�肳�ꂽ�{�^��ID��������Ă����Ԃ��`�F�b�N����
	if ((nowJoypadInput & buttonID) == 0)
	{
		return 1;
	}

	return 0;
}

//	�W���C�p�b�h�̃{�^���������ꂽ�u�Ԃ��`�F�b�N
int ExtendLibInput::IsButtonPressed(int buttonID)
{
	//	�w�肳�ꂽ�{�^��ID���A�O�t���[���ŉ�����Ă��炸�A���t���[���ŉ�����Ă�����A1��Ԃ�
	if ((nowJoypadInput & buttonID) == 1 &&
		(oldJoypadInput & buttonID) == 0)
	{
		return 1;
	}

	return 0;
}

//	�W���C�p�b�h�̃{�^���������ꂽ�u�Ԃ��`�F�b�N
int ExtendLibInput::IsButtonReleased(int buttonID)
{
	//	�w�肳�ꂽ�{�^��ID���A�O�t���[���ŉ�����Ă���A���t���[���ŉ�����Ă��Ȃ�������A1��Ԃ�
	if ((nowJoypadInput & buttonID) == 0 &&
		(oldJoypadInput & buttonID) == 1)
	{
		return 1;
	}


	return 0;
}

//	�L�[�{�[�h�̃{�^����������Ă��邩�`�F�b�N
int ExtendLibInput::IsKeyDown(int keyCode)
{
	//	�w�肳�ꂽ�L�[�R�[�h��������Ă����Ԃ��`�F�b�N����
	if (nowKeyInput[keyCode] == 1)
	{
		return 1;
	}

	return 0;
}

//	�L�[�{�[�h�̃{�^����������Ă��邩�`�F�b�N
int ExtendLibInput::IsKeyUp(int keyCode)
{
	//	�w�肳�ꂽ�L�[�R�[�h��������Ă����Ԃ��`�F�b�N����
	if (nowKeyInput[keyCode] == 0)
	{
		return 1;
	}

	return 0;
}

//	�L�[�{�[�h�̃{�^���������ꂽ�u�Ԃ��`�F�b�N
int ExtendLibInput::IsKeyPressed(int keyCode)
{
	//	�w�肳�ꂽ�L�[�R�[�h���A�O�t���[���ŉ�����Ă��炸�A���t���[���ŉ�����Ă�����A1��Ԃ�
	if (oldKeyInput[keyCode] == 0 &&
		nowKeyInput[keyCode] == 1)
	{
		return 1;
	}

	return 0;
}

//	�L�[�{�[�h�̃{�^���������ꂽ�u�Ԃ��`�F�b�N
int ExtendLibInput::IsKeyReleased(int keyCode)
{
	//	�w�肳�ꂽ�L�[�R�[�h���A�O�t���[���ŉ�����Ă���A���t���[���ŉ�����Ă��Ȃ�������A1��Ԃ�
	if (oldKeyInput[keyCode] == 1 &&
		nowKeyInput[keyCode] == 0)
	{
		return 1;
	}

	return 0;
}



