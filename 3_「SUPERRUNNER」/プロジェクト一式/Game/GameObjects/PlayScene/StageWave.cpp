//------------------------------------------//
//	����ҁF���ˍ_��						//
//	�T�@�v�F�����X�N���[���̃v���W�F�N�g	//
//------------------------------------------//

#include <DxLib.h>

#include "StageWave.h"
#include "Player.h"

//�R���X�g���N�^
StageWave::StageWave() :
	waveSpeed(0),
	waveWidth(0),
	positionX(0),
	ResetCount(0)
{
}

//�f�X�g���N�^
StageWave::~StageWave()
{
}

//������
void StageWave::Initialize(float x, float step, float ground)
{
	ResetCount = 0;
	boxObject[0].Initialize(x, step, x + waveWidth, step + 50);
	boxObject[1].Initialize(x, ground, x + waveWidth, 800);
}

//����̕��Z�b�g
void StageWave::SetWidth(int width)
{
	waveWidth = width;
}

//����̃X�s�[�h�Z�b�g
void StageWave::SetSpeed(int speed)
{
	waveSpeed = speed;
}

//�X�V
void StageWave::Update(float deltaTime)
{
	//�w��̑��x�ő���𓮂���
	for (int i = 0; i < 2; i++)
	{
		Position pos = boxObject[i].GetPosition();
		pos.x -= waveSpeed * deltaTime;
		boxObject[i].SetPosition(pos.x, pos.y);
	}
	positionX = boxObject[0].GetPosition().x;
}

//���Z�b�g�m�F
bool StageWave::ResetCheck(float pos)
{
	//����̈ʒu�ɓ��B�������̃`�F�b�N
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

//�`��
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
		//�����蔻�菈��
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

		//����Ɠ����������̏����i���ꂩ�猩�ăv���C���[���ǂ���ɂ��邩�j
		if (ret == BoxCollider::AABB_TOP)
		{
			//���W��ݒ�
			p.y = boxObject[i].GetBoxCollider().GetTop();

			if (player->GetVelocityY() < 0.0f)
			{
				player->ResetJumpCount();
				player->ResetJump();
			}
		}
		else if (ret == BoxCollider::AABB_BOTTOM)	//����ł���
		{
			//���W��ݒ�
			p.y = boxObject[i].GetBoxCollider().GetBottom() + 48;
			if (player->GetVelocityY() > 0.0f)
			{
				player->ResetJump();
			}
		}
		else if (ret == BoxCollider::AABB_LEFT)	//�E�ɕǂ�����
		{
			//���W��ݒ�
			p.x = boxObject[i].GetBoxCollider().GetLeft() - 24;
		}
		else if (ret == BoxCollider::AABB_RIGHT)	//���ɕǂ�����
		{
			//���W��ݒ�
			p.x = boxObject[i].GetBoxCollider().GetRight() + 24;
		}
		player->SetPosition(p);
	}
}

float StageWave::GetPositionX()
{
	return positionX;
}