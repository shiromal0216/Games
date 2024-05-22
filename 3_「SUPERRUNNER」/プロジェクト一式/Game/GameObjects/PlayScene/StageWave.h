//------------------------------------------//
//	制作者：松戸浩希						//
//	概　要：強制スクロールのプロジェクト	//
//------------------------------------------//

#pragma once

#include "BoxObject.h"

class Player;

class StageWave
{
public:

private:
	//加速用スピード
	int waveSpeed;
	//1Waveの幅
	int waveWidth;

	float positionX;

	//AABB当たり判定
	BoxObject boxObject[2];

	//Waveカウント
	int ResetCount;

public:
	StageWave();
	~StageWave();

	//初期化
	void Initialize(float x, float step, float ground);
	void SetWidth(int width);
	void SetSpeed(int speed);

	//更新
	void Update(float deltaTime);
	bool ResetCheck(float pos);
	//描画
	void Render();

	void CheckHitPlayer(Player* player);

	float GetPositionX();
};