//------------------------------------------//
//	制作者：松戸浩希						//
//	概　要：リザルトシーンヘッダー			//
//------------------------------------------//

#pragma once
#include "../../ExtendLib/GameSystemObject/GameSystemObjects.h"

class ResultScene
{
private:
	GameSystemObjects* pGameSystem;
	bool	changeSceneFlag;

	long haveScore;

public:
	ResultScene();												//	コンストラクタと呼ばれる関数
	~ResultScene();												//	デストラクタと呼ばれる関数


	void Initialize(int screenWidth, int screenHeight);			//	ゲームの初期化を行う関数
	void Update(float deltaTime);								//	ゲームの更新処理を行う関数
	void Render();												//	ゲームの描画を行う関数
	void Finalize();											//	ゲームの終了処理を行う関数

	void SetGameSystemObject(GameSystemObjects* pObj);
	void SetScore(int score);
	bool GetChangeSceneFlag();
};
