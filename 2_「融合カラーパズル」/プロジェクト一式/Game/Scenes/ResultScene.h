#pragma once
#include "../../ExtendLib/GameSystemObject/GameSystemObjects.h"
#include "../GameObjects/Block/Block.h"
#include "PlayScene.h"

class ResultScene
{
private:
	GameSystemObjects* pGameSystem;
	bool changeSceneFlag;

	int blockCount;
	
	bool mouseInputFlag;

	int ResultTextureID;

	int score;

	int mFade;
	bool mFadeFlag;

	int backcolor;

public:
	ResultScene();
	~ResultScene();

	void Initialize(int screenWidth, int screenHeight);
	void Update(float deltaTime);
	void Render();
	void Finalize();

	void SetGameSystemObject(GameSystemObjects* pObj);
	bool GetChangeSceneFlag();

	void SetBlockCount(int count);

	void SetScoreCount(int score);

};