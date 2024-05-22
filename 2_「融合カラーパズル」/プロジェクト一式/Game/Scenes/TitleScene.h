#pragma once
#include "../../ExtendLib/GameSystemObject/GameSystemObjects.h"
#include <DxLib.h>

class TitleScene
{
	enum class PhaseType
	{
		RenderTitle,
		RenderPlay,

		OverID
	};

	typedef struct Vector
	{
		float x;
		float y;
	}Position;

private:
public:

private:
	GameSystemObjects* pGameSystem;
	bool changeSceneFlag;
	bool mouseInputFlag;
public:
private:
	PhaseType phaseID;
	int titleCount;
	int titleTextureID;
	int titleTextureID2;
	int titlePushTextureID;

	int mFade;
	bool mFadeFlag;

	Vector titleLogoSize;
	Vector pushSpaceSize;

	int backcolor;

public:
	TitleScene();
	~TitleScene();

	void Initialize(int screenWidth,int screenHeight);
	void Update(float deltaTime);
	void Render();
	void Finalize();

	void SetGameSystemObject(GameSystemObjects* pObj);
	bool GetChangeSceneFlag();
};