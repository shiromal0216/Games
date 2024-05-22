#include <DxLib.h>
#include <cstdlib>
#include "PlayScene.h"

#define SCREEN_WIDTH		800
#define SCREEN_HEIGHT		600

#define FIELD_X				(290)
#define FIELD_Y				(100)
#define FIELD_WIDTH			(4)
#define FIELD_HEIGHT		(6)
#define BLOCK_SIZE			(60)

#define COUNTDOWN_NUM		(180)
#define COUNTDOWN_FONT_SIZE	(72)

PlayScene::PlayScene() :
	playerID(-1),
	saveID(-1),
	pGameSystem(nullptr),
	playState(PlayState::playerMove),
	ChangeSceneFlag(false),
	GameOver(false),
	mouseInputFlag(true),
	Time(0),
	playerX(-1),
	playerY(-1),
	answerColor(0),
	saveX(-1),
	saveY(-1),
	saveColor(0),
	blockcolor(0),
	blockcolor2(0),
	blockcolor3(0),
	score(0),
	backcolor(0),
	background(0),
	answerCount(0)
{
	for (int i = 0; i < FIELD_HEIGHT; i++) {
		for (int j = 0; j < FIELD_WIDTH; j++) {
			blocks[i][j].Initialize(j * BLOCK_SIZE + FIELD_X, i * BLOCK_SIZE + FIELD_Y);

			if (i >= 3 && i < FIELD_HEIGHT)
			{
				blocks[i][j].SetState(1);
				blocks[i][j].SetSingleColor(GetRand(2));
			}
		}
	}

	countDown = COUNTDOWN_NUM;

	//�摜�ǂݍ���
	playerID = LoadGraph("Resources/Textures/player.png");
	saveID = LoadGraph("Resources/Textures/save.png");

	//BGM��ǂݍ���
	bgmID = LoadSoundMem("Resources/Audio/����{�^��������49.mp3");
	bgmID2 = LoadSoundMem("Resources/Audio/NES-Action01-10(Item).mp3");
	bgmID3= LoadMusicMem("Resources/Audio/kakumei.wav");

	//�w�i�摜�ǂݍ���
	backcolor = LoadGraph("Resources/Textures/gamehaikei.png");
	background = LoadGraph("Resources/Textures/gameura.png");
}

PlayScene::~PlayScene()
{
	DeleteGraph(playerID);
	playerID = -1;
	DeleteGraph(saveID);
	saveID = -1;

	//BGM���������
	StopSoundMem(bgmID);
	DeleteSoundMem(bgmID);
	StopSoundMem(bgmID2);
	DeleteSoundMem(bgmID2);
	StopMusicMem(bgmID3);
	DeleteMusicMem(bgmID3);

	bgmID = -1;
	bgmID2 = -1;
	bgmID3 = -1;
}

void PlayScene::Initialize(float screenWidth, float screenHeight)
{
	ChangeSceneFlag = false;
	GameOver = false;
	Time = 0;
	waitTime = 0;
	blockcolor = LoadGraph("Resources/Textures/MixColor1.png");
	blockcolor2 = LoadGraph("Resources/Textures/MixColor2.png");
	blockcolor3 = LoadGraph("Resources/Textures/MixColor3.png");
	question = LoadGraph("Resources/Textures/odai.png");
	Description = LoadGraph("Resources/Textures/setumei.png");
	scores = LoadGraph("Resources/Textures/score.png");
	backcolor = LoadGraph("Resources/Textures/gamehaikei.png");
	background = LoadGraph("Resources/Textures/gameura.png");

	countDown = COUNTDOWN_NUM;

	score = 0;
	answerColor = GetRand(2);
	mouseInputFlag = true;

	mFade = 255;
	mFadeFlag = false;
	

	if (!CheckMusicMem(bgmID3))
	{
		//	BGM���܂����Ă��Ȃ���Ζ炷
		PlayMusicMem(bgmID3, DX_PLAYTYPE_LOOP);
		// ���ʒ���
		SetVolumeMusicMem(255 * 60 / 100, bgmID3);
	}

	for (int i = 0; i < FIELD_HEIGHT; i++) {
		for (int j = 0; j < FIELD_WIDTH; j++) {
			blocks[i][j].Initialize(j * BLOCK_SIZE + FIELD_X, i * BLOCK_SIZE + FIELD_Y);

			if (i >= 3 && i < FIELD_HEIGHT)
			{
				blocks[i][j].SetState(1);
				blocks[i][j].SetSingleColor(GetRand(2));
			}
		}
	}

	playerX = -1;
	playerY = -1;
	saveX = -1;
	saveY = -1;
	saveColor = 0;

	answerCount = 0;
}

void PlayScene::Update(float deltaTime)
{
	mFadeFlag = true;
	
	//���łɃQ�[���I�[�o�[�Ȃ珈�����Ȃ�
	if (GameOver == true)
	{
		return;
	}

	int mouseX, mouseY;

		if (mFadeFlag == true)
		{
			mFade -= 5;
		}

		if (mFade == 0)
		{
			mFade = 0;
			mFadeFlag = false;
			ChangeSceneFlag = true;
		}

	if (countDown == 0)
	{
	//�}�E�X�J�[�\���̍��W���擾
	GetMousePoint(&mouseX, &mouseY);

		if (playState == PlayState::playerMove) {
			//���Ԃ̌v��
			Time++;
			//������莞�Ԃ��o������
			if (Time % 300 == 0)
			{
				//�u���b�N�㏸
				if (BlockUp()) return;
			}

			//�v���C���[���W�̌���
			playerX = -1;
			playerY = -1;
			//�u���b�N�̐����J��Ԃ�
			for (int i = 0; i < FIELD_HEIGHT; i++) {
				for (int j = 0; j < FIELD_WIDTH; j++) {
					//�����J�[�\���ƃu���b�N���G��Ă�����
					if (blocks[i][j].CheckHitBlock((float)mouseX, (float)mouseY))
					{
						playerX = j;
						playerY = i;
						break;
					}
				}
				if (playerX != -1) break;
			}

			//�L�[��������Ă���Ȃ�
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
				//�N���b�N���Ȃ�
				if (!mouseInputFlag)
				{
					if (!CheckSoundMem(bgmID))
					{
						PlaySoundMem(bgmID, DX_PLAYTYPE_BACK);
					}

					mouseInputFlag = true;

					if (blocks[playerY][playerX].GetState() == 1) {
						//���݂̃u���b�N�̍��W,�F��ۑ�+��ʏ�Ԃ��ړ�
						saveX = playerX;
						saveY = playerY;
						saveColor = blocks[playerY][playerX].GetBlockSingleColor();
						playState = PlayState::playerSelect;
						return;
					}
				}
			}
			else
			{
				mouseInputFlag = false;
			}
		}
		else if (playState == PlayState::playerSelect) {
			//���Ԃ̌v��
			Time++;
			//������莞�Ԃ��o������
			if (Time % 300 == 0)
			{
				//�u���b�N�㏸
				if (BlockUp()) return;
			}

			//�v���C���[���W�̌���
			playerX = -1;
			playerY = -1;
			//�u���b�N�̐����J��Ԃ�
			for (int i = 0; i < FIELD_HEIGHT; i++) {
				for (int j = 0; j < FIELD_WIDTH; j++) {
					//�����J�[�\���ƃu���b�N���G��Ă�����
					if (blocks[i][j].CheckHitBlock((float)mouseX, (float)mouseY))
					{
						playerX = j;
						playerY = i;
						break;
					}
				}
			}

			//�L�[��������Ă���Ȃ�
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
				if (!mouseInputFlag)
				{
					if (blocks[saveY][saveX].GetBlockSingleColor() == blocks[playerY][playerX].GetBlockSingleColor()
						|| !(playerX >= saveX - 1 && playerX <= saveX + 1 && playerY == saveY
							|| playerY >= saveY - 1 && playerY <= saveY + 1 && playerX == saveX
							|| blocks[playerY][playerX].GetState() != 1)
						)
					{
						saveX = -1;
						saveY = -1;
						saveColor = 0;

						playState = PlayState::playerMove;
					}
					else
					{
						playState = PlayState::colorFusion;
						waitTime = 0;
					}
				}
			}
			else
			{
				mouseInputFlag = false;
			}
		}
		else if (playState == PlayState::colorFusion) {

			if (waitTime == 0) {

				if (!CheckSoundMem(bgmID2))
				{
					PlaySoundMem(bgmID2, DX_PLAYTYPE_BACK);
				}

				switch (blocks[saveY][saveX].GetBlockSingleColor()) {

				case 0:
					if (blocks[playerY][playerX].GetBlockSingleColor() == 1) blocks[playerY][playerX].SetMixColor(0);
					if (blocks[playerY][playerX].GetBlockSingleColor() == 2) blocks[playerY][playerX].SetMixColor(2);
					break;
				case 1:
					if (blocks[playerY][playerX].GetBlockSingleColor() == 0) blocks[playerY][playerX].SetMixColor(0);
					if (blocks[playerY][playerX].GetBlockSingleColor() == 2) blocks[playerY][playerX].SetMixColor(1);
					break;
				case 2:
					if (blocks[playerY][playerX].GetBlockSingleColor() == 0) blocks[playerY][playerX].SetMixColor(2);
					if (blocks[playerY][playerX].GetBlockSingleColor() == 1) blocks[playerY][playerX].SetMixColor(1);
					break;
				}

				blocks[playerY][playerX].SetState(2);
				blocks[saveY][saveX].SetState(3);
			}

			if (waitTime >= 60) {
				playState = PlayState::block;
				waitTime = 0;
			}
			waitTime++;

		}
		else if (playState == PlayState::block) {

			bool judgment = false;
			if (answerColor != blocks[playerY][playerX].GetBlockMixColor()) judgment = true;

			if (judgment == false)
			{
				blocks[playerY][playerX].SetState(3);
				////�ׂ荇�����u���b�N�����ז��u���b�N�Ȃ��u���b�N�ɕω�������
				//if (blocks[playerY][playerX + 1].GetBlockMixColor() == 3) blocks[playerY][playerX + 1].SetState(3);
				//if (blocks[playerY][playerX - 1].GetBlockMixColor() == 3) blocks[playerY][playerX - 1].SetState(3);
				//if (blocks[playerY + 1][playerX].GetBlockMixColor() == 3) blocks[playerY + 1][playerX].SetState(3);
				//if (blocks[playerY - 1][playerX].GetBlockMixColor() == 3) blocks[playerY - 1][playerX].SetState(3);
				answerCount++;

				//�X�R�AUP
				score += 100;
			}
			else
			{
				blocks[playerY][playerX].SetState(4);

				//�X�R�ADOWN
				score -= 100;
			}

			BlockDown();
			answerColor = GetRand(2);

			playState = PlayState::playerMove;
			saveX = -1;
			saveY = -1;
		}
	}
	else
	{
	countDown--;
 }
}

void PlayScene::Render()
{
	DrawGraph(0, 0, backcolor, 1);

	DrawGraph(245, 100, background, 1);

	DrawGraph(10, 90, question, 1);

	DrawGraph(20, 150, scores, 1);

	for (int i = 0; i < FIELD_HEIGHT; i++) {
		for (int j = 0; j < FIELD_WIDTH; j++) {
			if (blocks[i][j].GetState() != 0 && blocks[i][j].GetState() != 3)
			{
				blocks[i][j].Render();
			}
		}
	}

	if (playerX != -1) {
		DrawGraph(playerX * BLOCK_SIZE + FIELD_X, playerY * BLOCK_SIZE + FIELD_Y, playerID, 1);
	}
	if (saveX != -1) {
		DrawGraph(saveX * BLOCK_SIZE + FIELD_X, saveY * BLOCK_SIZE + FIELD_Y, saveID, 1);
	}

	//���~�b�g���C����\��
	DrawLine(230, 99, 600, 99, GetColor(250, 0, 0), 10);

	int defaultFontSize = GetFontSize();

	SetFontSize(36);
	if (answerColor == 0)
	{
		DrawGraph(150, 95, blockcolor, 1);
	}
	
	if (answerColor == 1)
	{
		DrawGraph(150, 95, blockcolor2, 1);
	}

	if (answerColor == 2)
	{
		DrawGraph(150, 95, blockcolor3, 1);
	}

	DrawFormatString(90, 165, GetColor(255, 255, 255), "\n%d", score);
	DrawGraph(590, 180, Description, 1);

	//	�J�E���g�_�E�����ł���΁A�J�E���^�[��\������
	if (countDown > 0)
	{
		//	�����̕����T�C�Y���擾
		int defaultStrSize = GetFontSize();

		//	�V���ȕ����T�C�Y��ݒ�
		SetFontSize(COUNTDOWN_FONT_SIZE);

		//	�J�E���^�[�̕\��
		DrawFormatString(SCREEN_WIDTH / 2 - COUNTDOWN_FONT_SIZE / 4, SCREEN_HEIGHT / 2 -
			COUNTDOWN_FONT_SIZE / 2, 0xFFFFFF, "%d", (countDown / 60 + 1));

		//	�����T�C�Y��߂�
		SetFontSize(defaultStrSize);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, mFade);
		DrawBoxAA(0, 0, 1000, 1000, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0.0);
	}

}

void PlayScene::Finalize()
{
	for (int i = 0; i < FIELD_HEIGHT; i++) 
	{
		for (int j = 0; j < FIELD_WIDTH; j++) 
		{
			blocks[i][j].Finalize();
		}
	}

	if (CheckSoundMem(bgmID))
	{
		//BGM���Ȃ��Ă����ԂȂ�~�߂�
		StopSoundMem(bgmID);
	}

	if (CheckSoundMem(bgmID2))
	{
		//BGM���Ȃ��Ă����ԂȂ�~�߂�
		StopSoundMem(bgmID2);
	}

	if (CheckSoundMem(bgmID3))
	{
		//BGM���Ȃ��Ă����ԂȂ�~�߂�
		StopSoundMem(bgmID3);
	}

	countDown = COUNTDOWN_NUM;
}

bool PlayScene::GetChangeSceneFlag()
{
	return GameOver;
}

void PlayScene::SetGameSystemObject(GameSystemObjects* pObj)
{
	pGameSystem = pObj;
}

 bool PlayScene::BlockUp()
{
	 //�����t�B�[���h�̈�ԏ�̃u���b�N�����݂���Ȃ�Q�[���I�[�o�[�ɂ��đ��I��
	 for (int j = 0; j < FIELD_WIDTH; j++)
	 {
		 if (blocks[0][j].GetState() != 0 )
		 {
				 GameOver = true;

		 }
	 }
	 //�t�B�[���h�̑S�Ẵu���b�N����i��ɏグ��
	 for (int i = 1; i < FIELD_HEIGHT; i++)
	 {
		 for (int j = 0; j < FIELD_WIDTH; j++)
		 {
			 blocks[i - 1][j].SetState(blocks[i][j].GetState());
			 blocks[i - 1][j].SetSingleColor(blocks[i][j].GetBlockSingleColor());
		 }
	 }
	 //��ԉ��Ƀ����_���Ńu���b�N�ǉ�
	 for (int j = 0; j < FIELD_WIDTH; j++)
	 {
		 blocks[5][j].SetState(1);
		 blocks[5][j].SetSingleColor(GetRand(2));
	 }
	 return true;
}

//�u���b�N�����ɉ�����
void PlayScene::BlockDown()
{
	for (int x = 0; x < FIELD_WIDTH; x++)
	{
		for (int y = FIELD_HEIGHT - 1; y >= 0; y--)
		{
			if (y - 1 < 0)
				break;

			if (blocks[y][x].GetState() == 0 || blocks[y][x].GetState() == 3)
			{
				for (int i = y - 1; i >= 0; i--)
				{
					if (blocks[i][x].GetState() != 3)
					{
						blocks[y][x].SetState(blocks[i][x].GetState());
						blocks[y][x].SetSingleColor(blocks[i][x].GetBlockSingleColor());
						blocks[i][x].SetState(0);

						y = i;
					}
				}
			}
		}
	}

	return;
}

int PlayScene::GetAnswerCount()
{
	return answerCount;
}

int PlayScene::GetScoreCount()
{
	return score;
}