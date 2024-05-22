#include "Collision.h"
#include <DXLib.h>

BoxCollider::BoxCollider()
	:x1(0),
	y1(0),
	x2(0),
	y2(0),
	width(0),
	height(0),
	leftTop{0,0},
	rightBottom{0,0},
	center{0,0}
{
}


BoxCollider::~BoxCollider()
{
}

void BoxCollider::Initialize(float ax1, float ay1, float ax2, float ay2)
{
	x1 = ax1;
	y1 = ay1;
	x2 = ax2;
	y2 = ay2;

	width = ax2 - ax1;
	height = ay2 - ay1;

	leftTop = Position{ x1,y1 };
	rightBottom = Position{ x2,y2 };
	center.x = (x1 + x2) / 2.0f;
	center.y = (y1 + y2) / 2.0f;


}

void BoxCollider::SetPosition(float x1, float y1)
{
	this->x1 = x1;
	this->y1 = y1;
	x2 = x1 + width;
	y2 = y1 + height;

}
void BoxCollider::SetScale(float width, float height)
{
	this->width = width;
	this->height = height;
}
unsigned int BoxCollider::CheckHit(float bx1, float by1, float bx2, float by2)
{
	// 戻り値
	unsigned int ret = AABB_NONE;

	// 当たり判定を取る
	if (!(x1 < bx2 && y1 < by2 && x2 > bx1 && y2 > by1))
	{
		// 当たってい無ければ処理しない
		return ret;
	}

	// 関数内の確認用フラグ
	unsigned int checkFlag = AABB_NONE;

	// めり込み具合の確認用変数
	float checkXRatio = 0.0f;
	float checkYRatio = 0.0f;

	// 相手(プレイヤー）の幅と高さを計算
	float width = bx2 - bx1;
	float height = by2 - by1;

	// 各方向のめり込み具合
	float rightRatio = (x2 - bx1) / width;
	float leftRatio = (bx2 - x1) / width;
	float downRatio = (y2 - by1) / height;
	float upRatio = (by2 - y1) / height;

	//	左右の比率から小さい方を記憶
	if (leftRatio < rightRatio)
	{
		// 左に寄っている
		checkFlag |= AABB_LEFT;
		checkXRatio = leftRatio;
	}
	else
	{
		// 右に寄っている
		checkFlag |= AABB_RIGHT;
		checkXRatio = rightRatio;
	}

	//	上下の比率から小さい方を記憶
	if (upRatio < downRatio)
	{
		// 上に寄っている
		checkFlag |= AABB_TOP;
		checkYRatio = upRatio;
	}
	else
	{
		// 下に寄っている
		checkFlag |= AABB_BOTTOM;
		checkYRatio = downRatio;
	}

	//	上下と左右で比率の小さい方を戻り値に入れる
	if (checkXRatio < checkYRatio )
	{
		// 左右のフラグを残す

		ret = checkFlag & (AABB_LEFT | AABB_RIGHT);
	}
	else if (checkXRatio > checkYRatio)
	{
		// 上下のフラグを残す
		ret = checkFlag & (AABB_TOP | AABB_BOTTOM);
	}

	return ret;
}

void BoxCollider::DebugRender()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	DrawBox(
		(int)x1,
		(int)y1,
		(int)x2,
		(int)y2,
		0xff0000,
		FALSE
	);
	DrawLine(
		(int)x1,
		(int)y1,
		(int)x2,
		(int)y2,
		0xff0000,
		FALSE
	);
	DrawLine(
		(int)x2,
		(int)y1,
		(int)x1,
		(int)y2,
		0xff0000,
		FALSE
	);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


CircleCollider::CircleCollider() :
	mRadius(0.0f),
	pos{0.0f, 0.0f}
{

}

CircleCollider::~CircleCollider()
{

}

bool CircleCollider::CheckHit(CircleCollider collider)
{
	//	2点の位置を原点からのベクトルにする
	float x = collider.pos.x - pos.x;
	float y = collider.pos.y - pos.y;

	//	ベクトルの長さを計算
	float l = (x * x) + (y * y);

	//	半径の合計を取得
	float circleLen = mRadius + collider.mRadius;

	//	半径の合計を2乗する(sqrtは処理が重いので使わないための処置)
	circleLen *= circleLen;

	//	ベクトルが2つの円の半径より短ければ当たっている
	if (l <= circleLen)
	{

		return true;
	}

	return false;
}

void CircleCollider::SetRadius(float newRadius)
{
	mRadius = newRadius;
}

void CircleCollider::SetPosition(Position newPos)
{
	pos = newPos;
}









