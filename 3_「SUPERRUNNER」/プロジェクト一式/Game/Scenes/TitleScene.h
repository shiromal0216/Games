//------------------------------------------//
//	制作者：松戸浩希						//
//	概　要：タイトルシーンヘッダー			//
//------------------------------------------//

#pragma once
#include "../../ExtendLib/GameSystemObject/GameSystemObjects.h"

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
	bool	changeSceneFlag;
public:

private:

	PhaseType phaseID;
	int titleCount;
	int titleTextureID;
	int titlePushTextureID;

	Vector titleLogoSize;
	Vector pushSpaceSize;

	//	変数宣言終了！

public:
	TitleScene();							//	コンストラクタと呼ばれる関数
	~TitleScene();							//	デストラクタと呼ばれる関数


	void Initialize(int screenWidth, int screenHeight);						//	ゲームの初期化を行う関数
	void Update(float deltaTime);					//	ゲームの更新処理を行う関数
	void Render();							//	ゲームの描画を行う関数。Drawと書く場合もある
	void Finalize();						//	ゲームの終了処理を行う関数。解放処理が必要な場合（Release関数などを呼び出す）は、この関数内で処理する

	void SetGameSystemObject(GameSystemObjects* pObj);
	bool GetChangeSceneFlag();
	//	関数宣言終了！
};
