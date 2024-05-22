/*
	WinMain.cpp
	プログラムのエントリーポイントと、ゲームのメインループを定義するファイル。
	終了キーであるEscキーの押下を監視し、Gameクラスの処理を呼び出す。


	作成者　：　堀川　和雅
	作成日　：　2021/03/15
*/

#include <DxLib.h>							//	DxLibエンジンを使うために必要なインクルード
#include "Game/Game.h"						//	Gameクラスを使用するために必要なインクルード
#include "ExtendLib/MemoryLeakDetector.h"
#include "ExtendLib/GameSystemObject/GameSystemObjects.h"

//*			//	この行の先頭が"//*"だと4:3、"/*"だと16:9になる
//	画面比を4:3にする
#define SCREEN_WIDTH						800					//	画面の横サイズ
#define SCREEN_HEIGHT						600					//	画面の縦サイズ
/*/
//	画面比を16:9にする
#define SCREEN_WIDTH						1280
#define SCREEN_HEIGHT						720
//*/

//	ウィンドウタイトルに表示する文字
#define GAME_TITLE							"SUPER RUNNER"


//	プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//	未使用引数の警告対策
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(nShowCmd);

	SetMainWindowText(GAME_TITLE);    // ウインドウタイトルの設定
#if defined(_DEBUG)
	//	メモリーリーク検出機構のセットアップ
	SetUpMemoryLeakDetector();
#endif
	
	SetOutApplicationLogValidFlag(FALSE);	//	デバッグ情報用ログファイルは出力しない

	//	起動モードの設定
	ChangeWindowMode(TRUE);					//	ウインドウモードで実行

	//	初期状態の画面モードの設定
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);

	//	DXライブラリの初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;							//	DxLibの初期化処理に失敗した
	}

	SetDrawScreen(DX_SCREEN_BACK);			//	画面の描画先を設定

	//	メインループ内で使用する変数を宣言する
	GameSystemObjects systemObject;
	Game gameJobObject;						//	ゲームの処理を実際に記述しているクラスオブジェクト

	//	宣言した変数の初期化が必要であれば、以下で行う
	systemObject.Initialize();
	
	gameJobObject.SetScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	gameJobObject.SetGameSystemObjects(&systemObject);
	gameJobObject.Initialize();				//	ゲームの処理を行う前に、初期化処理を行う

	//	メインループ。
	//	ESCキーが押されたら終了
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		//	ここにプログラムを書いていく --------------------------------------------
		//	特に指示がなければ、Game::Update()関数内に更新処理を書く
		systemObject.Update();
		gameJobObject.Update();

		//	描画処理。ゲームの描画処理はもう少し下で書く
		//	画面を初期化する。					描画処理の開始
		ClearDrawScreen();


		//	特に指示がなければ、Game::Render()関数内に描画処理を書く
		systemObject.Render();
		gameJobObject.Render();

		//	裏画面の内容を表画面に反映させる。	描画処理の終了
		ScreenFlip();

		//	フレーム内の処理終了！
		// --------------------------------------------------------------------------
	}
	//	この関数内で宣言した変数で、終了処理が必要なものがあれば、以下で行う
	gameJobObject.Finalize();				//	プログラムを終了する前に、ゲームの終了処理を行う

	systemObject.Finalize();

	InitGraph();

	//	DXライブラリの終了処理
	if (DxLib_End() == -1)
	{
		return -1;							//	DxLibの終了処理に失敗した
	}


	return 0;								//	正常終了
}
