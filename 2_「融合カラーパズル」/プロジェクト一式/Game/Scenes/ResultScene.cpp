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

	//”wŒi‰æ‘œ‚Ì“Ç‚Ýž‚Ý
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

	//ƒtƒF[ƒhƒCƒ“
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
	//”wŒi‰æ‘œ•`‰æ
	DrawGraph(0, 0, backcolor, 1);

	//ƒŠƒUƒ‹ƒg‰æ‘œ‚Ì•\Ž¦
	DrawGraph(0, 0, ResultTextureID, 1);

	int defaultFontSize = GetFontSize();

	SetFontSize(72);
	DrawFormatString(230, 170, GetColor(255, 0, 0), "GAME OVER");
	DrawFormatString(230, 250, GetColor(0, 0, 0), "SCOREF%d", score);

	SetFontSize(defaultFontSize);

	//ƒtƒF[ƒhƒCƒ“•`‰æ
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, mFade);
	DrawBoxAA(0, 0, 1000, 1000, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0.0);
}

void ResultScene::Finalize()
{
	//“Ç‚Ýž‚ñ‚¾‰æ‘œ‚Ì”jŠü
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