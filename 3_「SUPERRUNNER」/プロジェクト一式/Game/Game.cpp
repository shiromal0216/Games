/*
	�쐬�ҁ@�F�@���ˁ@�_��
	�쐬���@�F�@2022/7/25
*/

#include <math.h>
#include <DxLib.h>						//	������ł��g�����߁ADxLib.h���C���N���[�h���Ă���
#include "Game.h"

#define SCREEN_WIDTH			800	//��ʂ̉��T�C�Y
#define SCREEN_HEIGHT			600	//��ʂ̏c�T�C�Y

#define COUNTDOWN_NUM			(180)
#define COUNTDOWN_FONT_SIZE		(72)

//	�R���X�g���N�^�ƌĂ΂��֐�
Game::Game():
	sceneID(SceneID::TitleScene),
	screenWidth(0),
	screenHeight(0),
	oldTime((LONGLONG)0),
	nowTime((LONGLONG)0),
	deltaTime(0.0f),
	firstWait(false),
	pGameSystem(nullptr)
{
	//	����Ȃ������́A���̊֐����ɏ����͏����Ȃ���OK
}

//	�f�X�g���N�^�ƌĂ΂��֐�
Game::~Game()
{
	//	����Ȃ������́A���̊֐����ɏ����͏����Ȃ���OK
}


//	�Q�[���̏��������s���֐�
void Game::Initialize()
{
	//	�������珉���������������Ă����B
	//	�����ɏ��������́A�Q�[���N�������1�񂵂����s����Ȃ��悤�ɏ�����������Ă���B

	titleScene.Initialize(screenWidth, screenHeight);
	playScene.Initialize(screenWidth, screenHeight);
	resultScene.Initialize(screenWidth, screenHeight);
	countDown = COUNTDOWN_NUM;

	//	���������������I
}

//	�Q�[���̍X�V�������s���֐�
void Game::Update()
{
	//	�Q�[���̍X�V�����������Ă����B
	//	1�t���[����1����s�����悤�ɁA�Ăяo��������������Ă���B
	//	Render�֐����������Ă΂��B
	//	���̊֐����ł́A�`��̏������������Ƃ��ł��Ȃ��B
	oldTime = nowTime;
	nowTime = GetNowHiPerformanceCount();

	deltaTime = (float)((nowTime - oldTime) / 1000000.0f);

	bool changeSceneFlag = false;
	if (!firstWait && deltaTime < 1.0f)
	{
		firstWait = true;
	}
	else if (!firstWait)
	{
		return;
	}

	switch (sceneID)
	{
	case SceneID::TitleScene:
		titleScene.Update(deltaTime);
		changeSceneFlag = titleScene.GetChangeSceneFlag();
		break;
	case SceneID::PlayScene:
		playScene.Update(deltaTime);
		changeSceneFlag = playScene.GetChangeSceneFlag();
		break;
	case SceneID::ResultScene:
		resultScene.Update(deltaTime);
		changeSceneFlag = resultScene.GetChangeSceneFlag();
		break;
	}

	if (changeSceneFlag)
	{
		switch (sceneID)
		{
		case Game::SceneID::TitleScene:
			sceneID = SceneID::PlayScene;
			playScene.Initialize(screenWidth, screenHeight);
			break;
		case Game::SceneID::PlayScene:
			sceneID = SceneID::ResultScene;
			resultScene.Initialize(screenWidth, screenHeight);
			resultScene.SetScore(playScene.GetScore());
			break;
		case Game::SceneID::ResultScene:
			sceneID = SceneID::TitleScene;
			titleScene.Initialize(screenWidth, screenHeight);
			break;
		}
	}
	//	�Q�[���̍X�V�����I���I
}

//	�Q�[���̕`����s���֐��BDraw�Ə����ꍇ������
void Game::Render()
{
	//	�Q�[���̕`�揈���������Ă����B
	//	1�t���[����1����s�����悤�ɁA�Ăяo��������������Ă��āA
	//	���̊֐����Ă΂ꂽ�i�K�ł́A��ʂ����h��Ń��Z�b�g����Ă���B
	//	Update�֐��̌�ɌĂ΂��B
	//	���̊֐����ł́A�ł������v�Z�����Ȃǂ��s��Ȃ��悤�ɐS�����邱�ƁB
	//	�����Ȃ���Ώ��Ȃ��قǁA�������B


	switch (sceneID)
	{
	case Game::SceneID::TitleScene:
		titleScene.Render();
		break;
	case Game::SceneID::PlayScene:
		playScene.Render();
		break;
	case Game::SceneID::ResultScene:
		resultScene.Render();
		break;
	}


	//	�e�X�g�̕����`��

	//	�Q�[���̕`�揈���I���I
}

//	�Q�[���̏I���������s���֐��B����������K�v�ȏꍇ�iRelease�֐��Ȃǂ��Ăяo���j�́A���̊֐����ŏ�������
void Game::Finalize()
{
	//	�I�������������Ă����B
	//	�����ɏ��������́A�Q�[���I�����O��1�񂵂����s����Ȃ��悤�ɏ�����������Ă���B


	//	�I�����������I
}

void Game::SetScreenSize(int width, int height)
{
	screenWidth = width;
	screenHeight = height;
}

void Game::SetGameSystemObjects(GameSystemObjects* pObj)
{
	pGameSystem = pObj;

	titleScene.SetGameSystemObject(pObj);
	playScene.SetGameSystemObject(pObj);
	resultScene.SetGameSystemObject(pObj);
}