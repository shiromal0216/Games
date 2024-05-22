//------------------------------------------//
//	制作者：松戸浩希						//
//	概　要：タイトルシーン					//
//------------------------------------------//

#include "TitleScene.h"
#include <DxLib.h>

#define SCREEN_WIDTH		800	//画面の横サイズ
#define SCREEN_HEIGHT		600	//画面の縦サイズ

#define COUNTDOWN_NUM		60	//点滅の間隔時間（1秒）

TitleScene::TitleScene() :
	pGameSystem(nullptr),
	changeSceneFlag(false)
{
	//タイトルとプレイのフェーズの内、タイトル表示を設定する
	phaseID = PhaseType::RenderTitle;
	//PushKeyの表示点滅用カウンター
	titleCount = 0;

	//タイトル画像とPushKeyの画像読み込み
	titleTextureID = LoadGraph("Resources/Textures/Title.png");
	titlePushTextureID = LoadGraph("Resources/Textures/PushSpaceKey.png");
	
	//画像サイズをそれぞれ計測
	int sizeX, sizeY;

	// タイトル画像のX,Y軸の位置-------------------------
	GetGraphSize(titleTextureID, &sizeX, &sizeY);

	titleLogoSize.x = (float)sizeX;
	titleLogoSize.y = (float)sizeY;

	GetGraphSize(titlePushTextureID, &sizeX, &sizeY);

	pushSpaceSize.x = (float)sizeX;
	pushSpaceSize.y = (float)sizeY;
	//---------------------------------------------------
}

TitleScene::~TitleScene()
{

}

void TitleScene::Initialize(int screenWidth, int screenHeigh)
{
	changeSceneFlag = false;
}

void TitleScene::Update(float deltaTime)
{
	// スペースキーを押したらプレイシーンへ移動
	if (pGameSystem->GetInputSystem().IsKeyPressed(KEY_INPUT_SPACE))
		changeSceneFlag = true;
	// スペースキー画像を点滅させる
	titleCount++;
	if (titleCount > COUNTDOWN_NUM)
	{
		titleCount = 0;
	}
}

void TitleScene::Render()
{
	//タイトルロゴの表示
	DrawGraph(0, 0, titleTextureID, 1);

	if (titleCount >= 30)
	{
		//	30～59フレームであれば、PushKeyを表示
		DrawExtendGraph(
			(int)(SCREEN_WIDTH / 2.0f - SCREEN_WIDTH / 4.0f), (int)(SCREEN_HEIGHT / 2.0f + 100),
			(int)(SCREEN_WIDTH / 2.0f + SCREEN_WIDTH / 4.0f), (int)(SCREEN_HEIGHT / 2.0f + 150),
			titlePushTextureID, 1);
	}
}

void TitleScene::Finalize()
{
	//	読み込んだ画像の破棄
	DeleteGraph(titleTextureID);
	DeleteGraph(titlePushTextureID);

	titleTextureID = 0;
	titleTextureID = 0;
}

void TitleScene::SetGameSystemObject(GameSystemObjects* pObj)
{
	pGameSystem = pObj;
}

bool TitleScene::GetChangeSceneFlag()
{
	return changeSceneFlag;
}
