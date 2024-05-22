//------------------------------------------//
//	����ҁF���ˍ_��						//
//	�T�@�v�FAABB����̃v���W�F�N�g			//
//------------------------------------------//

#pragma once

#include "../../GameDatas/Utility.h"
#include "../../GameDatas/Collision.h"

class BoxObject
{
private:
	int boxID;

	//AABB�����蔻��
	BoxCollider boxCollider;

	//����̍��W(����̊p)
	Position pos;

public:
	BoxObject();
	~BoxObject();

	//������
	void Initialize(float x1, float y1, float x2, float y2);

	void SetPosition(float x1, float y1);

	void SetScale(float width, float height);

	//	�`��
	void Render();

	Position GetPosition();

	//�����蔻��̎擾
	BoxCollider GetBoxCollider();

	void Finalize();
};