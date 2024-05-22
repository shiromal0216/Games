//------------------------------------------//
//	制作者：松戸浩希						//
//	概　要：プレイシーンヘッダー			//
//------------------------------------------//

#pragma once
#include "../../ExtendLib/GameSystemObject/GameSystemObjects.h"

#include "../GameObjects/PlayScene/WaveManager.h"
#include "../GameObjects/PlayScene/Player.h"
#include"../GameObjects/PlayScene/BackGround.h"

class PlayScene
{
private:
	GameSystemObjects* pGameSystem;
	bool	changeSceneFlag;
	Player player;

	WaveManager waveManager;

	BackGround backGround;

	int meteo;	// メテオ
	int score;	// スコア

	bool ret;
	bool movemeteo;	// メテオの動く判定
	int meteoPositionX;	// メテオのX軸の位置
	int meteoPositionY;	// メテオのY軸の位置

	int bgmID;			// BGM

	int countDown;		// カウントダウン

public:
	PlayScene();												//	コンストラクタと呼ばれる関数
	~PlayScene();												//	デストラクタと呼ばれる関数


	void Initialize(int screenWidth, int screenHeight);			//	ゲームの初期化を行う関数
	void Update(float deltaTime);								//	ゲームの更新処理を行う関数
	void Render();												//	ゲームの描画を行う関数
	void Finalize();											//	ゲームの終了処理を行う関数

	void SetGameSystemObject(GameSystemObjects* pObj);
	bool GetChangeSceneFlag();

	bool CheckHitPlayer();										// プレイヤーとメテオの当たり判定

	int GetScore();												// スコアをゲットするための関数
};
