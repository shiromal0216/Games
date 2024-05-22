/*-------------------------
	作成者　：　松戸　浩希
	作成日　：　2022/08/06
---------------------------*/

#include "Game.h"
#include <DxLib.h>

#define SCREEN_WIDTH		800//画面の横サイズ
#define SCREEN_HEIGHT		600//画面の縦サイズ

Game::Game():
	sceneID(SceneID::TitleScene),
	screenWidth(0),
	screenHeight(0),
	oldTime((LONGLONG)0),
	nowTime(0.0f),
	firstWait(false),
	pGameSystem(nullptr)
{

}

Game::~Game()
{

}

void Game::Initialize()
{
	titleScene.Initialize(screenWidth, screenHeight);
	playScene.Initialize(screenWidth, screenHeight);
	resultScene.Initialize(screenWidth, screenHeight);
}

void Game::Update()
{
	oldTime = nowTime;
	nowTime = GetNowHiPerformanceCount();

	deltaTime = (float)((nowTime - oldTime) / 1000000.0f);

	bool changeSceneFlag = false;
	if (!firstWait && deltaTime < 1.0f)
	{
		firstWait = true;
	}
	else if (!firstWait)
	{
		return;
	}

	//現在のシーン更新を行う
	switch (sceneID)
	{
	case SceneID::TitleScene:
		titleScene.Update(deltaTime);
		changeSceneFlag = titleScene.GetChangeSceneFlag();
		break;
	case SceneID::PlayScene:
		playScene.Update(deltaTime);
		changeSceneFlag = playScene.GetChangeSceneFlag();
		break;
	case SceneID::ResultScene:
		resultScene.Update(deltaTime);
		changeSceneFlag = resultScene.GetChangeSceneFlag();
		break;
	}

		//シーン変更のタイミング
		if (changeSceneFlag)
		{
			//現在のシーンから次のシーンへ移動する
			switch (sceneID)
			{
			case Game::SceneID::TitleScene:
				sceneID = SceneID::PlayScene;
				playScene.Initialize(screenWidth, screenHeight);
				break;
			case Game::SceneID::PlayScene:
				sceneID = SceneID::ResultScene;
				resultScene.Initialize(screenWidth, screenHeight);

				//リザルトシーンにデータを渡す
				{
					int blockCount = playScene.GetAnswerCount();
					int scoreBoard = playScene.GetScoreCount();

					resultScene.SetBlockCount(blockCount);
					resultScene.SetScoreCount(scoreBoard);
				}
				
				break;
			case Game::SceneID::ResultScene:
				sceneID = SceneID::TitleScene;
				titleScene.Initialize(screenWidth, screenHeight);
				break;
			}
		}
	}

void Game::Render()
{
	switch (sceneID)
	{
	case Game::SceneID::TitleScene:
		titleScene.Render();
		break;
	case Game::SceneID::PlayScene:
		playScene.Render();
		break;
	case Game::SceneID::ResultScene:
		resultScene.Render();
		break;
	}

}

void Game::Finalize()
{

}

void Game::SetScreenSize(int width, int height)
{
	screenWidth = width;
	screenHeight = height;
}

void Game::SetGameSystemObject(GameSystemObjects* pObj)
{
	pGameSystem = pObj;

	titleScene.SetGameSystemObject(pObj);
	playScene.SetGameSystemObject(pObj);
	resultScene.SetGameSystemObject(pObj);
}