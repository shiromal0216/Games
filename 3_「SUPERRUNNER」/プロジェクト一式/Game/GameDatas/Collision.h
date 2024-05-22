#pragma once

#include "Utility.h"

class BoxCollider
{
public:
	const static int AABB_NONE = 0;
	const static int AABB_TOP = 1 << 0;
	const static int AABB_RIGHT = 1 << 1;
	const static int AABB_BOTTOM = 1 << 2;
	const static int AABB_LEFT = 1 << 3;


private:
	// ��`�̍�����W�ƉE�����W
	float x1;
	float y1;
	float x2;
	float y2;

	float width;
	float height;

	Position leftTop;
	Position rightBottom;
	Position center;

public:
	BoxCollider();
	void Initialize(float ax1, float ay1, float ax2, float ay2);
	~BoxCollider();
	// �����蔻��iAABB�j
	unsigned int CheckHit(float bx1, float by1, float bx2, float by2);
	float GetLeft() { return x1; }
	float GetRight() { return x2; }
	float GetTop() { return y1; }
	float GetBottom() { return y2; }

	void SetPosition(float x1, float y1);
	void SetScale(float width, float height);
	void DebugRender();


	Position GetLeftTop() { return leftTop; }


};


// �~�̔���p�\����
class CircleCollider
{
private:
	// ���a
	float mRadius;

	// ���W
	Position pos;

	// �֐�	================================
public:
	CircleCollider();
	~CircleCollider();
	// ���̉~�Ƃ̓����蔻��
	bool CheckHit(CircleCollider collider);
	void SetRadius(float newRadius);
	void SetPosition(Position newPos);
};