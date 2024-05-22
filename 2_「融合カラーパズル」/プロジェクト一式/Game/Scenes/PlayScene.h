#pragma once

#include "../../ExtendLib/GameSystemObject/GameSystemObjects.h"
#include "../GameObjects/Block/Block.h"

class PlayScene
{
private:
	enum class PlayState {
		playerMove,
		playerSelect,
		colorFusion,
		block,

		OverID
	};

	GameSystemObjects* pGameSystem;
	Block blocks[6][4];
	int playerID;
	int saveID;
	PlayState playState;

	bool ChangeSceneFlag;
	bool GameOver;
	bool mouseInputFlag;
	int Time;
	int waitTime;

	int playerX;
	int playerY;
	int answerColor;
	int saveX;
	int saveY;
	int saveColor;
	int answerCount;
	int bgmID;
	int bgmID2;
	int bgmID3;

	int blockcolor;
	int blockcolor2;
	int blockcolor3;

	int score;

	int Description;
	int question;
	int scores;

	int countDown;	//試合開始前のカウントダウン

	int mFade;
	bool mFadeFlag;

	int backcolor;

	int background;

public:
	PlayScene();
	~PlayScene();

	void Initialize(float screenWidth, float screenHeight);
	void Update(float deltaTime);
	void Render();
	void Finalize();

	bool GetChangeSceneFlag();
	void SetGameSystemObject(GameSystemObjects* pObj);

	bool BlockUp();
	void BlockDown();

	int GetAnswerCount();

	int GetScoreCount();
};