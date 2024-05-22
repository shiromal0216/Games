/*
	GameSystemObjects.cpp
	�Q�[���Ŏg����{�I�ȋ@�\�̃I�u�W�F�N�g���W�߂��N���X�B

	�쐬�ҁ@�F�@�x��@�a��
	�쐬���@�F�@2021/03/15
*/
#include "GameSystemObjects.h"

//	�R���X�g���N�^
GameSystemObjects::GameSystemObjects() :
	pInputSystem(nullptr),
	isInitialize(false)
{

}

//	�f�X�g���N�^
GameSystemObjects::~GameSystemObjects()
{
	pInputSystem = nullptr;
}

//	����������
void GameSystemObjects::Initialize()
{
	pInputSystem = new ExtendLibInput();
	pInputSystem->Initialize();

	pTimerObject = new ExtendLibTime();

	isInitialize = true;
}

//	�X�V����
void GameSystemObjects::Update()
{
	if (!isInitialize)
	{
		//	����������Ă��Ȃ���΁A�X�V���������Ȃ�
		return;
	}

	//	�{�^���V�X�e���̍X�V���s��
	pTimerObject->UpdateFrameTimer();
	pInputSystem->Update();
}

//	�`�揈��
void GameSystemObjects::Render()
{

}

//	�I������
void GameSystemObjects::Finalize()
{
	delete pTimerObject;
	pTimerObject = nullptr;
	delete pInputSystem;
	pInputSystem = nullptr;
}


//	�C���v�b�g�����p�N���X�I�u�W�F�N�g�̎擾
ExtendLibInput& GameSystemObjects::GetInputSystem()
{
	return (*pInputSystem);
}

//	�^�C�}�[�����p�N���X�I�u�W�F�N�g�̎擾
ExtendLibTime& GameSystemObjects::GetTimerSystem()
{
	return (*pTimerObject);
}

//	�������ς݂��������t���O�̓��e���擾����
bool GameSystemObjects::GetInitializeFlag()
{
	return isInitialize;
}
