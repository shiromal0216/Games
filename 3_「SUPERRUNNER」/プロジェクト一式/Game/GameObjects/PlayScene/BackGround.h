//------------------------------------------//
//	制作者：松戸浩希						//
//	概　要：背景の色を時間ごとに変える		//
//------------------------------------------//

#pragma once

#include "../../GameDatas/Utility.h"

class BackGround
{
public:
	//雲の数
	const static int cloudNum = 5;

private:
	int mountID;	//山の画像ハンドル
	int cloudID;	//雲の画像ハンドル

	float mountPos[2];		//山の座標（２枚）
	Position cloudPos[cloudNum];	//雲の座標（雲の数分）

	int speed;	//背景の流れる速度

public:
	BackGround();
	~BackGround();

	void Initialize();
	void Update(float deltaTime);
	void Render();

	void Finalize();

	void SetSpeed(int speed);
};