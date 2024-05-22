//------------------------------------------//
//	����ҁF���ˍ_��						//
//	�T�@�v�F�����X�N���[���̃v���W�F�N�g	//
//------------------------------------------//

#pragma once

#include "BoxObject.h"

class Player;

class StageWave
{
public:

private:
	//�����p�X�s�[�h
	int waveSpeed;
	//1Wave�̕�
	int waveWidth;

	float positionX;

	//AABB�����蔻��
	BoxObject boxObject[2];

	//Wave�J�E���g
	int ResetCount;

public:
	StageWave();
	~StageWave();

	//������
	void Initialize(float x, float step, float ground);
	void SetWidth(int width);
	void SetSpeed(int speed);

	//�X�V
	void Update(float deltaTime);
	bool ResetCheck(float pos);
	//�`��
	void Render();

	void CheckHitPlayer(Player* player);

	float GetPositionX();
};