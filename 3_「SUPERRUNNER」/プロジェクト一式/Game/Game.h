/*
	作成者　：　松戸　浩希
	作成日　：　2022/07/25
*/
#pragma once
#include "../ExtendLib/GameSystemObject/GameSystemObjects.h"
#include "Scenes/TitleScene.h"
#include "Scenes/PlayScene.h"
#include "Scenes/ResultScene.h"

/*
	ゲームの処理を実装するために作成するクラス。
	プロのゲームプログラマーは、このクラスと呼ばれる書き方を駆使してプログラムを作成している。
*/
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
	//	処理を組む際、複数の関数で使用したい変数を宣言する。
	//	Game::関数名()と書かれた関数なら、どこでも使用可能。
	//	public:より下でも変数宣言できるが、基本的に指示がなければ、
	//	ゲームプログラミングの授業ではpublic:よりも上で変数宣言すること！
	//	また、一つの関数の中でしか使わない変数は、関数の中で宣言すること！

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

	int countDown;	//ゲーム開始前のカウントダウン

	GameSystemObjects* pGameSystem;

	//	変数宣言終了！
public:
	Game();									//	コンストラクタと呼ばれる関数
	~Game();								//	デストラクタと呼ばれる関数

	void Initialize();						//	ゲームの初期化を行う関数
	void Update();							//	ゲームの更新処理を行う関数
	void Render();							//	ゲームの描画を行う関数。Drawと書く場合もある
	void Finalize();						//	ゲームの終了処理を行う関数。解放処理が必要な場合（Release関数などを呼び出す）は、この関数内で処理する

	void SetScreenSize(int width, int height);

	void SetGameSystemObjects(GameSystemObjects* pObj);

	//	関数宣言終了！
};

