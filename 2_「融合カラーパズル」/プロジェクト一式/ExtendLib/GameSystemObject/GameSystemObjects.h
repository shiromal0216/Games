/*
	GameSystemObjects.h
	�Q�[���Ŏg����{�I�ȋ@�\�̃I�u�W�F�N�g���W�߂��N���X�B

	�쐬�ҁ@�F�@�x��@�a��
	�쐬���@�F�@2021/03/15
*/
#pragma once
#include "../ExtendLibInput/ExtendLibInput.h"
#include "../ExtendLibTime/ExtendLibTime.h"

class GameSystemObjects
{
private:
	ExtendLibInput* pInputSystem;
	ExtendLibTime* pTimerObject;
	bool isInitialize;

public:
	GameSystemObjects();					//	�R���X�g���N�^
	~GameSystemObjects();					//	�f�X�g���N�^

	void Initialize();						//	����������
	void Update();							//	�X�V����
	void Render();							//	�`�揈��
	void Finalize();						//	�I������

	ExtendLibInput& GetInputSystem();		//	�C���v�b�g�����p�N���X�I�u�W�F�N�g�̎擾
	ExtendLibTime& GetTimerSystem();		//	�^�C�}�[�����p�N���X�I�u�W�F�N�g�̎擾
	bool GetInitializeFlag();				//	�������ς݂��������t���O�̓��e���擾����
};