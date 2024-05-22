#include "TitleScene.h"
#include <DxLib.h>

#define SCREEN_WIDTH	800//画面横サイズ
#define SCREEN_HEIGHT	600//画面縦サイズ

TitleScene::TitleScene() :
	pGameSystem(nullptr),
	mouseInputFlag(true),
	changeSceneFlag(false),
	backcolor(0)
{
	//タイトルとプレイのフェーズの内、タイトル表示を設定する
	phaseID = PhaseType::RenderTitle;
	//TitleとPushKeyの表示点滅用カウンター
	titleCount = 0;

	//タイトル画像とPushKeyの画像読み込み
	titleTextureID = LoadGraph("Resources/Textures/Title.png");
	titleTextureID2 = LoadGraph("Resources/Textures/Title2.png");
	titlePushTextureID = LoadGraph("Resources/Textures/Click The Screen.png");

	//背景画像の読み込み
	backcolor = LoadGraph("Resources/Textures/haikei.png");

	//画像サイズをそれぞれ計測
	int sizeX, sizeY;

	GetGraphSize(titleTextureID, &sizeX, &sizeY);

	titleLogoSize.x = (float)sizeX;
	titleLogoSize.y = (float)sizeY;

	GetGraphSize(titleTextureID2, &sizeX, &sizeY);

	titleLogoSize.x = (float)sizeX;
	titleLogoSize.y = (float)sizeY;

	GetGraphSize(titlePushTextureID, &sizeX, &sizeY);

	pushSpaceSize.x = (float)sizeX;
	pushSpaceSize.y = (float)sizeY;
}

TitleScene::~TitleScene()
{

}

void TitleScene::Initialize(int screenWidth, int screenHeight)
{
	changeSceneFlag = false;
	mouseInputFlag = true;

	mFade = 0;
	mFadeFlag = false;
}

void TitleScene::Update(float deltaTime)
{
	int mouseX, mouseY;

	//マウスカーソルの座標を取得
	GetMousePoint(&mouseX, &mouseY);

	//キーが押されているなら
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
	titleCount++;
	if (titleCount > 60)
	{
		titleCount = 0;
	}

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

void TitleScene::Render()
{
	//背景画像描画
	DrawGraph(0, 0, backcolor, 1);

	//タイトル画像入れ替え
	DrawGraph(0, 0, titleTextureID, 1);

	if (titleCount >= 30)
	{
		DrawGraph(0, 0, titleTextureID2, 1);
	}

	//タイトルロゴの表示

	if (titleCount >= 30)
	{
		DrawExtendGraph(
			(int)(SCREEN_WIDTH / 2.0f - SCREEN_WIDTH / 4.0f), (int)(SCREEN_HEIGHT / 2.0f + 100),
			(int)(SCREEN_WIDTH / 2.0f + SCREEN_WIDTH / 4.0f), (int)(SCREEN_HEIGHT / 2.0f + 150),
			titlePushTextureID, 1);
	}
	
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, mFade);
	DrawBoxAA(0, 0, 1000, 1000, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0.0);
}

void TitleScene::Finalize()
{
	//読み込んだ画像の破棄
	DeleteGraph(titleTextureID);
	DeleteGraph(titleTextureID2);
	DeleteGraph(titlePushTextureID);
	DeleteGraph(backcolor);

	titleTextureID = 0;
	titleTextureID2 = 0;
}

void TitleScene::SetGameSystemObject(GameSystemObjects* pObj)
{
	pGameSystem = pObj;
}

bool TitleScene::GetChangeSceneFlag()
{
	return changeSceneFlag;
}