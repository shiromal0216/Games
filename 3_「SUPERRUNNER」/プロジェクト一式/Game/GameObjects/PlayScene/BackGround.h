//------------------------------------------//
//	����ҁF���ˍ_��						//
//	�T�@�v�F�w�i�̐F�����Ԃ��Ƃɕς���		//
//------------------------------------------//

#pragma once

#include "../../GameDatas/Utility.h"

class BackGround
{
public:
	//�_�̐�
	const static int cloudNum = 5;

private:
	int mountID;	//�R�̉摜�n���h��
	int cloudID;	//�_�̉摜�n���h��

	float mountPos[2];		//�R�̍��W�i�Q���j
	Position cloudPos[cloudNum];	//�_�̍��W�i�_�̐����j

	int speed;	//�w�i�̗���鑬�x

public:
	BackGround();
	~BackGround();

	void Initialize();
	void Update(float deltaTime);
	void Render();

	void Finalize();

	void SetSpeed(int speed);
};