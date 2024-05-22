//------------------------------------------//
//	����ҁF���ˍ_��						//
//	�T�@�v�FAABB����̃v���W�F�N�g			//
//------------------------------------------//

#include <DxLib.h>

#include "BoxObject.h"

#define CHIP_SIZE	(16)

//�R���X�g���N�^

BoxObject::BoxObject() :
	pos{ 0,0 }
{
	//�摜��ǂݍ���
	boxID = LoadGraph("Resources/Textures/ground.png");
}

//�f�X�g���N�^

BoxObject::~BoxObject()
{
}

//������

void BoxObject::Initialize(float x1, float y1, float x2, float y2)
{
	//�����蔻��̏�����
	boxCollider.Initialize(x1, y1, x2, y2);
	pos.x = x1;
	pos.y = y1;
}

void BoxObject::SetPosition(float x1,float y1)
{
	boxCollider.SetPosition(x1, y1);

	pos.x = x1;
	pos.y = y1;
}

Position BoxObject::GetPosition()
{
	return pos;
}

void BoxObject::SetScale(float width, float height)
{
	boxCollider.SetScale(width, height);
}

//�`��

void BoxObject::Render()
{
	//Box�ŕ`��
	int left = (int)boxCollider.GetLeft();
	int right = (int)boxCollider.GetRight();
	int top = (int)boxCollider.GetTop();
	int bottom = (int)boxCollider.GetBottom();

	int width = right - left;
	int height = bottom - top;

	DrawBox(
		left,
		top,
		right,
		bottom,
		GetColor(150, 255, 150),
		TRUE
	);

	for (int j = 1; j < height / CHIP_SIZE; j++)
	{
		for (int i = 1; i < width / CHIP_SIZE; i++)
		{
			DrawRectRotaGraph3(
				(int)left + i * CHIP_SIZE,
				(int)top + j * CHIP_SIZE,
				CHIP_SIZE, CHIP_SIZE,
				CHIP_SIZE, CHIP_SIZE,
				0, 0,
				1, 1,
				0.0,
				boxID,
				TRUE);

		}
	}
	for (int i = 1; i < width / CHIP_SIZE; i++)
	{
		//���
		DrawRectRotaGraph3(
			(int)left + i * CHIP_SIZE,
			(int)top,
			CHIP_SIZE, 0,
			CHIP_SIZE, CHIP_SIZE,
			0, 0,
			1, 1,
			0.0,
			boxID,
			TRUE);

		//����
		DrawRectRotaGraph3(
			(int)left + i * CHIP_SIZE,
			(int)bottom,
			CHIP_SIZE, CHIP_SIZE * 2,
			CHIP_SIZE, CHIP_SIZE,
			0, CHIP_SIZE,
			1, 1,
			0.0,
			boxID,
			TRUE);
	}
	for (int j = 1; j < height / CHIP_SIZE; j++)
	{
		//����
		DrawRectRotaGraph3(
			(int)left,
			(int)top + j * CHIP_SIZE,
			0, CHIP_SIZE,
			CHIP_SIZE, CHIP_SIZE,
			0, 0,
			1, 1,
			0.0,
			boxID,
			TRUE);
		//�E��
		DrawRectRotaGraph3(
			(int)right,
			(int)top + j * CHIP_SIZE,
			CHIP_SIZE * 2, CHIP_SIZE,
			CHIP_SIZE, CHIP_SIZE,
			CHIP_SIZE, 0,
			1, 1,
			0.0,
			boxID,
			TRUE);
	}
	//����̊p
	DrawRectRotaGraph3(
		(int)left, (int)top,
		0, 0,
		CHIP_SIZE, CHIP_SIZE,
		0, 0,
		1, 1,
		0.0,
		boxID,
		TRUE);
	//�E��̊p
	DrawRectRotaGraph3(
		(int)right, (int)top,
		CHIP_SIZE * 2, 0,
		CHIP_SIZE, CHIP_SIZE,
		CHIP_SIZE, 0,
		1, 1,
		0.0,
		boxID,
		TRUE);
	//�����̊p
	DrawRectRotaGraph3(
		(int)left, (int)bottom,
		0, CHIP_SIZE * 2,
		CHIP_SIZE, CHIP_SIZE,
		0, CHIP_SIZE,
		1, 1,
		0.0,
		boxID,
		TRUE);
	//�E���̊p
	DrawRectRotaGraph3(
		(int)right, (int)bottom,
		CHIP_SIZE * 2, CHIP_SIZE * 2,
		CHIP_SIZE, CHIP_SIZE,
		CHIP_SIZE, CHIP_SIZE,
		1, 1,
		0.0,
		boxID,
		TRUE);
	
}

//�߂�l:AABB���̓������\����
BoxCollider BoxObject::GetBoxCollider()
{
	return boxCollider;
}

void BoxObject::Finalize()
{
	DeleteGraph(boxID);
}
