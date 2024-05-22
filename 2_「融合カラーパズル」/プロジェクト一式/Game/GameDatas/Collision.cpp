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
	// �߂�l
	unsigned int ret = AABB_NONE;

	// �����蔻������
	if (!(x1 < bx2 && y1 < by2 && x2 > bx1 && y2 > by1))
	{
		// �������Ă�������Ώ������Ȃ�
		return ret;
	}

	// �֐����̊m�F�p�t���O
	unsigned int checkFlag = AABB_NONE;

	// �߂荞�݋�̊m�F�p�ϐ�
	float checkXRatio = 0.0f;
	float checkYRatio = 0.0f;

	// ����(�v���C���[�j�̕��ƍ������v�Z
	float width = bx2 - bx1;
	float height = by2 - by1;

	// �e�����̂߂荞�݋
	float rightRatio = (x2 - bx1) / width;
	float leftRatio = (bx2 - x1) / width;
	float downRatio = (y2 - by1) / height;
	float upRatio = (by2 - y1) / height;

	//	���E�̔䗦���珬���������L��
	if (leftRatio < rightRatio)
	{
		// ���Ɋ���Ă���
		checkFlag |= AABB_LEFT;
		checkXRatio = leftRatio;
	}
	else
	{
		// �E�Ɋ���Ă���
		checkFlag |= AABB_RIGHT;
		checkXRatio = rightRatio;
	}

	//	�㉺�̔䗦���珬���������L��
	if (upRatio < downRatio)
	{
		// ��Ɋ���Ă���
		checkFlag |= AABB_TOP;
		checkYRatio = upRatio;
	}
	else
	{
		// ���Ɋ���Ă���
		checkFlag |= AABB_BOTTOM;
		checkYRatio = downRatio;
	}

	//	�㉺�ƍ��E�Ŕ䗦�̏���������߂�l�ɓ����
	if (checkXRatio < checkYRatio )
	{
		// ���E�̃t���O���c��

		ret = checkFlag & (AABB_LEFT | AABB_RIGHT);
	}
	else if (checkXRatio > checkYRatio)
	{
		// �㉺�̃t���O���c��
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
	//	2�_�̈ʒu�����_����̃x�N�g���ɂ���
	float x = collider.pos.x - pos.x;
	float y = collider.pos.y - pos.y;

	//	�x�N�g���̒������v�Z
	float l = (x * x) + (y * y);

	//	���a�̍��v���擾
	float circleLen = mRadius + collider.mRadius;

	//	���a�̍��v��2�悷��(sqrt�͏������d���̂Ŏg��Ȃ����߂̏��u)
	circleLen *= circleLen;

	//	�x�N�g����2�̉~�̔��a���Z����Γ������Ă���
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









