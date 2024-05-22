//------------------------------------------//
//	制作者：松戸浩希						//
//	概　要：強制スクロールのプロジェクト	//
//------------------------------------------//

#include <DxLib.h>

#include "StageWave.h"
#include "Player.h"

//コンストラクタ
StageWave::StageWave() :
	waveSpeed(0),
	waveWidth(0),
	positionX(0),
	ResetCount(0)
{
}

//デストラクタ
StageWave::~StageWave()
{
}

//初期化
void StageWave::Initialize(float x, float step, float ground)
{
	ResetCount = 0;
	boxObject[0].Initialize(x, step, x + waveWidth, step + 50);
	boxObject[1].Initialize(x, ground, x + waveWidth, 800);
}

//足場の幅セット
void StageWave::SetWidth(int width)
{
	waveWidth = width;
}

//足場のスピードセット
void StageWave::SetSpeed(int speed)
{
	waveSpeed = speed;
}

//更新
void StageWave::Update(float deltaTime)
{
	//指定の速度で足場を動かす
	for (int i = 0; i < 2; i++)
	{
		Position pos = boxObject[i].GetPosition();
		pos.x -= waveSpeed * deltaTime;
		boxObject[i].SetPosition(pos.x, pos.y);
	}
	positionX = boxObject[0].GetPosition().x;
}

//リセット確認
bool StageWave::ResetCheck(float pos)
{
	//特定の位置に到達したかのチェック
	bool ret = false;
	for (int i = 0; i < 2; i++)
	{
		if (boxObject[i].GetPosition().x < pos)
		{
			ret = true;
		}
	}

	return ret;
}

//描画
void StageWave::Render()
{
	for (int i = 0; i < 2; i++)
	{
		boxObject[i].Render();
	}
}

void StageWave::CheckHitPlayer(Player* player)
{
	for (int i = 0; i < 2; i++)
	{
		Position p = player->GetPosition();
		//当たり判定処理
		//BoxCollider GetBoxCollider;
		unsigned int ret = boxObject[i].GetBoxCollider().CheckHit(
			p.x - 24,
			p.y - 48,
			p.x + 24,
			p.y
		);
		if (ret == 0)
		{
			continue;
		}

		//足場と当たった時の処理（足場から見てプレイヤーがどちらにあるか）
		if (ret == BoxCollider::AABB_TOP)
		{
			//座標を設定
			p.y = boxObject[i].GetBoxCollider().GetTop();

			if (player->GetVelocityY() < 0.0f)
			{
				player->ResetJumpCount();
				player->ResetJump();
			}
		}
		else if (ret == BoxCollider::AABB_BOTTOM)	//頭を打った
		{
			//座標を設定
			p.y = boxObject[i].GetBoxCollider().GetBottom() + 48;
			if (player->GetVelocityY() > 0.0f)
			{
				player->ResetJump();
			}
		}
		else if (ret == BoxCollider::AABB_LEFT)	//右に壁がある
		{
			//座標を設定
			p.x = boxObject[i].GetBoxCollider().GetLeft() - 24;
		}
		else if (ret == BoxCollider::AABB_RIGHT)	//左に壁がある
		{
			//座標を設定
			p.x = boxObject[i].GetBoxCollider().GetRight() + 24;
		}
		player->SetPosition(p);
	}
}

float StageWave::GetPositionX()
{
	return positionX;
}