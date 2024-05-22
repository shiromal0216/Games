/*-------------------------
	�쐬�ҁ@�F�@���ˁ@�_��
	�쐬���@�F�@2022/08/06
---------------------------*/
#pragma once
#include "../ExtendLib/GameSystemObject/GameSystemObjects.h"

#include "Scenes/PlayScene.h"
#include "Scenes/ResultScene.h"
#include "Scenes/TitleScene.h"
class Game
{
public:
	enum class SceneID
	{
		TitleScene,
		PlayScene,
		ResultScene,
		OverID
	};

private:
	int screenWidth;
	int screenHeight;

	SceneID sceneID;

	TitleScene titleScene;
	PlayScene playScene;
	ResultScene resultScene;

	LONGLONG oldTime;
	LONGLONG nowTime;
	float deltaTime;

	bool firstWait;

	GameSystemObjects* pGameSystem;

public:

	Game();
	~Game();

	void Initialize();
	void Update();
	void Render();
	void Finalize();
	void SetScreenSize(int width, int height);
	void SetGameSystemObject(GameSystemObjects* pObj);

};

