//--------------------------------------//
//	����ҁF���ˍ_��					//
//	�T�@�v�F�v���C�V�[��				//
//--------------------------------------//

#include "PlayScene.h"
#include <DxLib.h>

#define SCREEN_WIDTH			800		//��ʂ̉��T�C�Y
#define SCREEN_HEIGHT			600		//��ʂ̏c�T�C�Y
 
#define COUNTDOWN_NUM			(180)	// �J�E���g�_�E���̕b��(�R�b)
#define COUNTDOWN_FONT_SIZE		(72)	// �t�H���g�T�C�Y



PlayScene::PlayScene() :
	score(0),
	pGameSystem(nullptr),
	changeSceneFlag(false),
	movemeteo(false),
	ret(false),
	countDown(COUNTDOWN_NUM),
	meteoPositionX(900),
	meteoPositionY(200)
{
	// �摜�̓ǂݍ���
	meteo = LoadGraph("Resources/Textures/meteo.png");
	// ���y�̓ǂݍ���
	bgmID = LoadMusicMem("Resources/Audio/bunmeikaika.wav");
}

PlayScene::~PlayScene()
{
	StopMusicMem(bgmID);
	DeleteMusicMem(bgmID);
	bgmID = -1;
}

void PlayScene::Initialize(int screenWidth, int screenHeigh)
{
	backGround.Initialize();
	waveManager.Initialize();
	player.Initialize();
	meteoPositionX = 900;
	meteoPositionY = 200;
	movemeteo = false;
	countDown = COUNTDOWN_NUM;
	changeSceneFlag = false;
	score = 0;

	// �~���[�W�b�N���Đ�������
	if (!CheckMusicMem(bgmID))
	{
		PlayMusicMem(bgmID, DX_PLAYTYPE_LOOP);
	}
}

void PlayScene::Update(float deltaTime)
{
	if (countDown < 0)
	{
		// �J�E���g�_�E����0�b�Ȃ�Q�[�����J�n����
		backGround.Update(deltaTime);
		waveManager.Update(deltaTime);
		player.Update();
		waveManager.CheckHitPlayer(&player);
		backGround.SetSpeed(waveManager.GetSpeed());
		CheckHitPlayer();

		if (player.GetPosition().y > 650 || player.GetPosition().x > 1000 || ret == true)
		{
			changeSceneFlag = true;
		}

		score++;

	}
	else
	{
		countDown--;
	}
	
	// �i�񂾋�����1000���[�g���𒴂�����
	if (score >= 1000)
	{
		movemeteo = true;
	}
	
	// ���e�I����ʉE�[����o��������
	if (movemeteo == true)
	{
		// ���e�I�̐i�ދ���
		meteoPositionX -= 15;
	}


	if (meteoPositionX <= -110)
	{
		meteoPositionX = 900;
		meteoPositionY = GetRand(250) + 50;
	}
}

void PlayScene::Render()
{
	backGround.Render();
	waveManager.Render();
	player.Render();

	//�J�E���g�_�E�����ł���΁A�J�E���^�[��\������
	if (countDown > 0)
	{
		//�����̕����T�C�Y���擾
		int defaultStrSize = GetFontSize();

		//�V���ȕ����T�C�Y��ݒ�
		SetFontSize(COUNTDOWN_FONT_SIZE);

		//�J�E���^�[�̕\��
		DrawFormatString(SCREEN_WIDTH / 2 - COUNTDOWN_FONT_SIZE / 4, SCREEN_HEIGHT / 2 -
			COUNTDOWN_FONT_SIZE / 2, 0xff0000, "%d", (countDown / 60 + 1));

		//�����T�C�Y��߂�
		SetFontSize(defaultStrSize);
	}

	DrawGraph(meteoPositionX,meteoPositionY,meteo,1);
}

void PlayScene::Finalize()
{
	player.Finalize();
	changeSceneFlag = false;

	// �~���[�W�b�N�̔j��
	if (CheckMusicMem(bgmID))
	{
		StopMusicMem(bgmID);
	}
}

void PlayScene::SetGameSystemObject(GameSystemObjects* pObj)
{
	pGameSystem = pObj;
	player.SetSystemObject(pObj);
}

bool PlayScene::GetChangeSceneFlag()
{
	return changeSceneFlag;
}

int PlayScene::GetScore()
{
	// �X�R�A��Ԃ�
	return score;
}

bool PlayScene::CheckHitPlayer()
{
	// �v���C���[�ƃ��e�I�̓����蔻��
	ret = false;
	Position playerPos = player.GetPosition();
	if (playerPos.x >= meteoPositionX + 20 &&
		playerPos.y >= meteoPositionY + 30 &&
		playerPos.x <= meteoPositionX + 135 &&
		playerPos.y <= meteoPositionY + 120)
	{
		ret = true;
	}

	return ret;
}