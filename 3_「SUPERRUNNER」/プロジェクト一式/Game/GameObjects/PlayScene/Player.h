//------------------------------------------//
//	制作者：松戸浩希						//
//	概　要：プレイヤーのプロジェクト		//
//------------------------------------------//

#pragma once

#include "../../GameDatas/Utility.h"
#include "../../GameDatas/Collision.h"
#include "../../../ExtendLib/GameSystemObject/GameSystemObjects.h"

class Player
{
public:
	const static int MAX_JUMP = 2;

private:
	//位置
	Position position;

	//速度
	Vector velocity;

	//加速度
	float acceleration;

	//重力
	float gravity;

	//ジャンプの可能数
	int jumpNum;
	//2段ジャンプ時の回転
	int jumpRot;
	//プレイヤー画像のハンドル
	int textureHandle;

	GameSystemObjects* pSystemObject;

	//歩行時のアニメーションキー
	int animationKey;

	//生存フラグ
	bool isAlive;

public:
	Player();
	~Player();

	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Render();

	Position GetPosition();
	float GetVelocityY();

	void SetSystemObject(GameSystemObjects* pObj);

	//ジャンプの速度の初期化（頭打ちや地面）
	void ResetJump();
	//ジャンプ回数の初期化（地面）
	void ResetJumpCount();

	void SetPosition(Position pos);

	void Finalize();

	// 生存かどうか
	bool IsAlive();
};