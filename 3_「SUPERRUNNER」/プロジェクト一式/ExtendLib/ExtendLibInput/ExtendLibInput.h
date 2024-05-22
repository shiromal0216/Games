/*
	ExtendLibInput.h
	DxLib�𗘗p���āA���͂̏�Ԃ��Ǘ�����N���X�B
	DxLib�ł́A�{�^����������Ă��邩������Ă��Ȃ����͕����邪�A
	�����ꂽ�u�Ԃ��A�����ꂽ�u�Ԃ��A�Ƃ����`�F�b�N���ł��Ȃ����߁A
	���̕������J�o�[���邽�߂̏������T�|�[�g����B


	�쐬�ҁ@�F�@�x��@�a��
	�쐬���@�F�@2021/03/15
*/
#pragma once

class ExtendLibInput
{
private:
	static const int KEY_CODE_MAX;

	int nowJoypadInput;					//	�W���C�p�b�h�̌��݂̓���
	int oldJoypadInput;					//	�W���C�p�b�h��1�t���[���O�̓���

	int nowKeyInput[256];				//	�L�[�R�[�h�̌��݂̓���
	int oldKeyInput[256];				//	�L�[�R�[�h��1�t���[���O�̓���

public:
	ExtendLibInput();					//	�R���X�g���N�^
	~ExtendLibInput();					//	�f�X�g���N�^

	void Initialize();					//	����������
	void Update();						//	�X�V����
	void Finalize();					//	�I������

	int IsButtonDown(int buttonID);		//	�W���C�p�b�h�̃{�^����������Ă��邩�`�F�b�N
	int IsButtonUp(int buttonID);		//	�W���C�p�b�h�̃{�^����������Ă��邩�`�F�b�N
	int IsButtonPressed(int buttonID);	//	�W���C�p�b�h�̃{�^���������ꂽ�u�Ԃ��`�F�b�N
	int IsButtonReleased(int buttonID);	//	�W���C�p�b�h�̃{�^���������ꂽ�u�Ԃ��`�F�b�N

	int IsKeyDown(int keyCode);			//	�L�[�{�[�h�̃{�^����������Ă��邩�`�F�b�N
	int IsKeyUp(int keyCode);			//	�L�[�{�[�h�̃{�^����������Ă��邩�`�F�b�N
	int IsKeyPressed(int keyCode);		//	�L�[�{�[�h�̃{�^���������ꂽ�u�Ԃ��`�F�b�N
	int IsKeyReleased(int keyCode);		//	�L�[�{�[�h�̃{�^���������ꂽ�u�Ԃ��`�F�b�N
};

