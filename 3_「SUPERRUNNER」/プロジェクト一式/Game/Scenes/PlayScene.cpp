//--------------------------------------//
//	制作者：松戸浩希					//
//	概　要：プレイシーン				//
//--------------------------------------//

#include "PlayScene.h"
#include <DxLib.h>

#define SCREEN_WIDTH			800		//画面の横サイズ
#define SCREEN_HEIGHT			600		//画面の縦サイズ
 
#define COUNTDOWN_NUM			(180)	// カウントダウンの秒数(３秒)
#define COUNTDOWN_FONT_SIZE		(72)	// フォントサイズ



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
	// 画像の読み込み
	meteo = LoadGraph("Resources/Textures/meteo.png");
	// 音楽の読み込み
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

	// ミュージックを再生させる
	if (!CheckMusicMem(bgmID))
	{
		PlayMusicMem(bgmID, DX_PLAYTYPE_LOOP);
	}
}

void PlayScene::Update(float deltaTime)
{
	if (countDown < 0)
	{
		// カウントダウンが0秒ならゲームを開始する
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
	
	// 進んだ距離が1000メートルを超えたら
	if (score >= 1000)
	{
		movemeteo = true;
	}
	
	// メテオを画面右端から出現させる
	if (movemeteo == true)
	{
		// メテオの進む距離
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

	//カウントダウン中であれば、カウンターを表示する
	if (countDown > 0)
	{
		//初期の文字サイズを取得
		int defaultStrSize = GetFontSize();

		//新たな文字サイズを設定
		SetFontSize(COUNTDOWN_FONT_SIZE);

		//カウンターの表示
		DrawFormatString(SCREEN_WIDTH / 2 - COUNTDOWN_FONT_SIZE / 4, SCREEN_HEIGHT / 2 -
			COUNTDOWN_FONT_SIZE / 2, 0xff0000, "%d", (countDown / 60 + 1));

		//文字サイズを戻す
		SetFontSize(defaultStrSize);
	}

	DrawGraph(meteoPositionX,meteoPositionY,meteo,1);
}

void PlayScene::Finalize()
{
	player.Finalize();
	changeSceneFlag = false;

	// ミュージックの破棄
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
	// スコアを返す
	return score;
}

bool PlayScene::CheckHitPlayer()
{
	// プレイヤーとメテオの当たり判定
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