//------------------------------------------//
//	制作者：松戸浩希						//
//	概　要：プレイヤーのプロジェクト		//
//------------------------------------------//

#include <DxLib.h>

#include "Player.h"
#include <math.h>
#include <float.h>

//プレイヤーの初期位置
#define START_POSITION_X	(200)
#define START_POSITION_Y	(500)

//画面の幅
#define SCREEN_HEIGHT	(800)

//プレイヤーの画像の１キャラ分の大きさ
#define CHIP_WIDTH	(32)
#define CHIP_HEIGHT	(32)

//プレイヤーの表示拡大率
#define PLAYER_WIDTH_RATE	(1.5f)
#define PLAYER_HEIGHT_RATE	(1.5f)

//画面上のプレイヤーのサイズ
#define PLAYER_WIDTH (int)(CHIP_WIDTH * PLAYER_WIDTH_RATE)
#define PLAYER_HEIGHT (int)(CHIP_HEIGHT * PLAYER_HEIGHT_RATE)

//画像の中心位置
#define ANCHOR_X (CHIP_WIDTH / 2)
#define ANCHOR_Y (CHIP_HEIGHT)

//円周率
#define PI					(3.14159265358979323846)

//コンストラクタ
Player::Player():
	position{0.0f,0.0f},
	velocity{ 0.0f,0.0f },
	acceleration(0.0f),
	gravity(0.0f),
	jumpNum(0),
	jumpRot(0),
	pSystemObject(nullptr),
	animationKey(1),
	isAlive(true)
{
	// 画像読み込み
	textureHandle = LoadGraph("Resources/Textures/Character.png");
}

//デストラクタ
Player::~Player()
{
}

//初期化
void Player::Initialize()
{
	position.x = START_POSITION_X;
	position.y = START_POSITION_Y;

	velocity.x = 0.0f;
	velocity.y = 0.0f;

	acceleration = 0.0f;

	gravity = 0.0f;

	jumpNum = MAX_JUMP;

	jumpRot = 0;

	animationKey = 1;

	isAlive = true;
}

Position Player::GetPosition()
{
	return position;
}
float Player::GetVelocityY()
{
	return velocity.y + gravity;
}

//更新
void Player::Update()
{
	float x = 0.0f;
	float y = 0.0f;

	velocity.x = 0.0f;

	// プレイヤーの初期位置
	if (position.x < START_POSITION_X)
	{
		acceleration = (START_POSITION_X - position.x) * 0.01f;
		velocity.x = 1.0f;

		if (acceleration > 1.0f)
		{
			acceleration = 1.0f;
		}
	}
	//簡易摩擦処理
	if (fabsf(velocity.x) < FLT_EPSILON)
	{
		acceleration *= 0.8f;
	}

	//上キー入力でジャンプ
	if (pSystemObject->GetInputSystem().IsKeyPressed(KEY_INPUT_UP) == TRUE && jumpNum > 0)
	{
		velocity.y = 6.0f;
		gravity = 0.0f;

		//ジャンプ回数の消費
		jumpNum--;
	}

	//速度＋加速度のベクトルを作る
	x = velocity.x + acceleration;

	//重力加速度を設定
	if (pSystemObject->GetInputSystem().IsKeyDown(KEY_INPUT_UP) == TRUE)
	{
		gravity -= 0.1f;
	}
	else
	{
		gravity -= 0.3f;
	}
	//ジャンプベクトルを作る
	y = velocity.y + gravity;

	//Yベクトルを反転
	y *= -1.0f;

	//座標にベクトルを加算
	position.x += x;
	position.y += y;

	if (position.y < PLAYER_HEIGHT)
	{
		position.y = PLAYER_HEIGHT;
	}

	// 死ぬ判定を返す
	if (position.y > SCREEN_HEIGHT + PLAYER_WIDTH)
	{
		isAlive = false;
	}
}

//描画
void Player::Render()
{
	int interval = 10;

	int keys[4] = { 0,1,2,1 };

	if (jumpNum == 0)
	{
		//２段目のジャンプは回転する
		jumpRot += 10;
		DrawRectRotaGraph3(
			(int)position.x,
			(int)position.y - (PLAYER_HEIGHT / 2),
			0,
			CHIP_HEIGHT * 2,
			CHIP_WIDTH,
			CHIP_HEIGHT,
			ANCHOR_X,
			ANCHOR_Y - (CHIP_HEIGHT / 2),
			PLAYER_HEIGHT_RATE,
			PLAYER_WIDTH_RATE,
			jumpRot * PI / 180.0,
			textureHandle,
			TRUE);
	}
	else if (jumpNum == 1)
	{
		DrawRectRotaGraph3(
			(int)position.x,
			(int)position.y,
			0,
			CHIP_HEIGHT * 2,
			CHIP_WIDTH,
			CHIP_HEIGHT,
			ANCHOR_X,
			ANCHOR_Y,
			1.5,
			1.5,
			0.0,
			textureHandle,
			TRUE);
	}
	else
	{
		//地面にいるときは腕を振る
		DrawRectRotaGraph3(
			(int)position.x,
			(int)position.y,
			CHIP_WIDTH * keys[animationKey / interval],
			CHIP_HEIGHT * 2,
			CHIP_WIDTH,
			CHIP_HEIGHT,
			ANCHOR_X,
			ANCHOR_Y,
			1.5,
			1.5,
			0.0,
			textureHandle,
			TRUE);
		jumpRot = 0;
		animationKey = (animationKey + 1) % (4 * interval);
	}
	
}

void Player::SetSystemObject(GameSystemObjects* pObj)
{
	pSystemObject = pObj;
}

void Player::ResetJumpCount()
{
	//ジャンプ回数のリセット
	jumpNum = MAX_JUMP;
}

void Player::ResetJump()
{
	//重力とジャンプ力のリセット
	velocity.y = 0.0f;
	gravity = 0.0f;
}

void Player::SetPosition(Position pos)
{
	position = pos;
}

void Player::Finalize()
{
	DeleteGraph(textureHandle);
}

// 生存しているかどうか
bool Player::IsAlive()
{
	return isAlive;
}