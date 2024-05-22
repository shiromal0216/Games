/*
	WinMain.cpp
	�v���O�����̃G���g���[�|�C���g�ƁA�Q�[���̃��C�����[�v���`����t�@�C���B
	�I���L�[�ł���Esc�L�[�̉������Ď����AGame�N���X�̏������Ăяo���B


	�쐬�ҁ@�F�@�x��@�a��
	�쐬���@�F�@2021/03/15
*/

#include <DxLib.h>							//	DxLib�G���W�����g�����߂ɕK�v�ȃC���N���[�h
#include "Game/Game.h"						//	Game�N���X���g�p���邽�߂ɕK�v�ȃC���N���[�h
#include "ExtendLib/MemoryLeakDetector.h"
#include "ExtendLib/GameSystemObject/GameSystemObjects.h"

//*			//	���̍s�̐擪��"//*"����4:3�A"/*"����16:9�ɂȂ�
//	��ʔ��4:3�ɂ���
#define SCREEN_WIDTH						800					//	��ʂ̉��T�C�Y
#define SCREEN_HEIGHT						600					//	��ʂ̏c�T�C�Y
/*/
//	��ʔ��16:9�ɂ���
#define SCREEN_WIDTH						1280
#define SCREEN_HEIGHT						720
//*/

//	�E�B���h�E�^�C�g���ɕ\�����镶��
#define GAME_TITLE							"SUPER RUNNER"


//	�v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//	���g�p�����̌x���΍�
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(nShowCmd);

	SetMainWindowText(GAME_TITLE);    // �E�C���h�E�^�C�g���̐ݒ�
#if defined(_DEBUG)
	//	�������[���[�N���o�@�\�̃Z�b�g�A�b�v
	SetUpMemoryLeakDetector();
#endif
	
	SetOutApplicationLogValidFlag(FALSE);	//	�f�o�b�O���p���O�t�@�C���͏o�͂��Ȃ�

	//	�N�����[�h�̐ݒ�
	ChangeWindowMode(TRUE);					//	�E�C���h�E���[�h�Ŏ��s

	//	������Ԃ̉�ʃ��[�h�̐ݒ�
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);

	//	DX���C�u�����̏���������
	if (DxLib_Init() == -1)
	{
		return -1;							//	DxLib�̏����������Ɏ��s����
	}

	SetDrawScreen(DX_SCREEN_BACK);			//	��ʂ̕`����ݒ�

	//	���C�����[�v���Ŏg�p����ϐ���錾����
	GameSystemObjects systemObject;
	Game gameJobObject;						//	�Q�[���̏��������ۂɋL�q���Ă���N���X�I�u�W�F�N�g

	//	�錾�����ϐ��̏��������K�v�ł���΁A�ȉ��ōs��
	systemObject.Initialize();
	
	gameJobObject.SetScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	gameJobObject.SetGameSystemObjects(&systemObject);
	gameJobObject.Initialize();				//	�Q�[���̏������s���O�ɁA�������������s��

	//	���C�����[�v�B
	//	ESC�L�[�������ꂽ��I��
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		//	�����Ƀv���O�����������Ă��� --------------------------------------------
		//	���Ɏw�����Ȃ���΁AGame::Update()�֐����ɍX�V����������
		systemObject.Update();
		gameJobObject.Update();

		//	�`�揈���B�Q�[���̕`�揈���͂����������ŏ���
		//	��ʂ�����������B					�`�揈���̊J�n
		ClearDrawScreen();


		//	���Ɏw�����Ȃ���΁AGame::Render()�֐����ɕ`�揈��������
		systemObject.Render();
		gameJobObject.Render();

		//	����ʂ̓��e��\��ʂɔ��f������B	�`�揈���̏I��
		ScreenFlip();

		//	�t���[�����̏����I���I
		// --------------------------------------------------------------------------
	}
	//	���̊֐����Ő錾�����ϐ��ŁA�I���������K�v�Ȃ��̂�����΁A�ȉ��ōs��
	gameJobObject.Finalize();				//	�v���O�������I������O�ɁA�Q�[���̏I���������s��

	systemObject.Finalize();

	InitGraph();

	//	DX���C�u�����̏I������
	if (DxLib_End() == -1)
	{
		return -1;							//	DxLib�̏I�������Ɏ��s����
	}


	return 0;								//	����I��
}
