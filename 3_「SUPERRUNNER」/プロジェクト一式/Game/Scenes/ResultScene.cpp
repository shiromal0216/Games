//------------------------------------------//
//	制作者：松戸浩希						//
//	概　要：リザルトシーン					//
//------------------------------------------//

#include <DxLib.h>
#include "ResultScene.h"

ResultScene::ResultScene() :
	haveScore(0),
	pGameSystem(nullptr),
	changeSceneFlag(false)
{

}

ResultScene::~ResultScene()
{

}

void ResultScene::Initialize(int screenWidth, int screenHeigh)
{
	changeSceneFlag = false;
}

void ResultScene::Update(float deltaTime)
{
	//スペースキーが押されていたら、フェードアウトを行う
	if (pGameSystem->GetInputSystem().IsKeyPressed(KEY_INPUT_SPACE))
	{
		changeSceneFlag = true;
	}
}

void ResultScene::Render()
{
	int defaultFontSize = GetFontSize();

	// リザルトシーンの文字表示
	SetFontSize(54);
	DrawFormatString(170, 180, GetColor(255, 255, 255), "進んだ距離：");
	DrawFormatString(480, 180, GetColor(255, 255, 255), "%dm", haveScore);
	DrawFormatString(130, 300, GetColor(255, 255, 255), "評価：");
	DrawFormatString(130, 400, GetColor(255, 255, 255), "スペースでタイトルへ");

	// 進んだ距離によって評価を変える
	if (haveScore < 500)
	{
		DrawString(320, 300, "まだまだだね！", GetColor(255, 255, 255), 1);
	}
	else if(haveScore < 1999)
	{
		DrawString(320, 300, "なかなかだね！", GetColor(255, 255, 255), 1);
	}
	else
	{
		DrawString(320, 300, "素晴らしい！", GetColor(255, 255, 255), 1);
	}

	SetFontSize(defaultFontSize);
}

void ResultScene::Finalize()
{
	
}

void ResultScene::SetGameSystemObject(GameSystemObjects* pObj)
{
	pGameSystem = pObj;
}

bool ResultScene::GetChangeSceneFlag()
{
	// フラグを返す
	return changeSceneFlag;
}

void ResultScene::SetScore(int score)
{
	// スコアをセットする
	haveScore = score;
}