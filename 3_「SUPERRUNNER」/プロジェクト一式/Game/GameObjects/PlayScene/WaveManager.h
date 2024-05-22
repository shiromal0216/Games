//------------------------------//
//	制作者：松戸浩希			//
//	概　要：足場の生成			//
//------------------------------//

#pragma once
#include "StageWave.h"

class Player;

class WaveManager
{
private:
	enum class FOOT_POS {
		None,
		Bottom,
		Middle,
		Top,

		OverID
	};
	const static int STEP_TYPE = 4;

public:
	const static int WAVE_WIDTH = 200;
	const static int WAVE_SPEED = 500;
	const static int WAVES = 5;

private:
	float stepPosition[STEP_TYPE];
	float groundPosition[STEP_TYPE];

	StageWave stageWave[WAVES];

	int resetCount;

	FOOT_POS lastGroundPos;

	int speed;

public:
	WaveManager();
	~WaveManager();

	void Initialize();

	void Update(float deltaTime);
	void CheckHitPlayer(Player* p);

	void Render();

	int GetSpeed();
};