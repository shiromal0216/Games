#include <DxLib.h>
#include "ResultScene.h"

ResultScene::ResultScene() :
	blockCount(0),
	score(0),
	backcolor(0),
	changeSceneFlag(false),
	mouseInputFlag(true),
	pGameSystem(nullptr)
{
	ResultTextureID = LoadGraph("Resources/Textures/Result.png");

	//�w�i�摜�̓ǂݍ���
	backcolor = LoadGraph("Resources/Textures/haikei.png");
}

ResultScene::~ResultScene()
{
}

void ResultScene::Initialize(int screenWidth, int screenHeight)
{
	blockCount = 0;
	score = 0;
	changeSceneFlag = false;
	mouseInputFlag = true;

	mFade = 0;
	
	mFadeFlag = false;
}

void ResultScene::Update(float deltaTime)
{	
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		if (!mouseInputFlag)
		{
			mFadeFlag = true;
		}
		
	}
	else
	{
		mouseInputFlag = false;
	}

	//�t�F�[�h�C��
	if (mFadeFlag == true)
	{
		mFade += 5;
	}

	if (mFade == 255)
	{
		mFade = 0;
		mFadeFlag = false;
		changeSceneFlag = true;
	}
}

void ResultScene::Render()
{
	//�w�i�摜�`��
	DrawGraph(0, 0, backcolor, 1);

	//���U���g�摜�̕\��
	DrawGraph(0, 0, ResultTextureID, 1);

	int defaultFontSize = GetFontSize();

	SetFontSize(72);
	DrawFormatString(230, 170, GetColor(255, 0, 0), "GAME OVER");
	DrawFormatString(230, 250, GetColor(0, 0, 0), "SCORE�F%d", score);

	SetFontSize(defaultFontSize);

	//�t�F�[�h�C���`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, mFade);
	DrawBoxAA(0, 0, 1000, 1000, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0.0);
}

void ResultScene::Finalize()
{
	//�ǂݍ��񂾉摜�̔j��
	DeleteGraph(ResultTextureID);
	DeleteGraph(backcolor);

	ResultTextureID = 0;
}

bool ResultScene::GetChangeSceneFlag()
{
	return changeSceneFlag;
}

void ResultScene::SetGameSystemObject(GameSystemObjects* pObj)
{
	pGameSystem = pObj;
}

void ResultScene::SetBlockCount(int count)
{
	blockCount = count;
}

void ResultScene::SetScoreCount(int score)
{
	this->score = score;
}