//------------------------------//
//	����ҁF���ˍ_��			//
//	�T�@�v�F����̐���			//
//------------------------------//

#include "WaveManager.h"
#include "Player.h"
#include <DxLib.h>

//�R���X�g���N�^
WaveManager::WaveManager():
	stepPosition{1000,300,250,200},
	groundPosition{ 1000,500,450,400 },
	resetCount(0),
	speed(0),
	lastGroundPos(FOOT_POS::Bottom)
{
	for (int i = 0; i < WAVES; i++)
	{
		stageWave[i].SetSpeed(WAVE_SPEED);
		stageWave[i].SetWidth(WAVE_WIDTH);
		stageWave[i].Initialize(i * 200.0f, stepPosition[(int)FOOT_POS::None],
			groundPosition[(int)FOOT_POS::Bottom]);
	}
}

//�f�X�g���N�^
WaveManager::~WaveManager()
{
}

//������
void WaveManager::Initialize()
{
	for (int i = 0; i < WAVES; i++)
	{
		stageWave[i].SetSpeed(WAVE_SPEED);
		stageWave[i].SetWidth(WAVE_WIDTH);
		stageWave[i].Initialize(i * 200.0f, stepPosition[(int)FOOT_POS::None],
			groundPosition[(int)FOOT_POS::Bottom]);
	}
	speed = WAVE_SPEED;
}

//�X�V
void WaveManager::Update(float deltaTime)
{
	//����̍X�V
	for (int i = 0; i < WAVES; i++)
	{
		stageWave[i].Update(deltaTime);
	}
	//���ꂪ��ʊO�ɏ��������ԉE���̑���̂����ׂɔz�u����
	for (int i = 0; i < WAVES; i++)
	{
		if (stageWave[i].ResetCheck(-WAVE_WIDTH))
		{
			float resetPosX = stageWave[(WAVES + i - 1) % WAVES].GetPositionX() + WAVE_WIDTH;
		
			FOOT_POS step = (FOOT_POS)(rand() % 4);

			//��̑���͂T�O��(���m�ɂ�37.55%)�ŏo��
			if (rand() % 2)
			{
				step = FOOT_POS::None;
			}
			FOOT_POS ground = (FOOT_POS)(rand() % 4);

			//���̑���͂Q�A���Ō��ɂȂ�Ȃ��悤�ɂ���
			if (ground == FOOT_POS::None)
			{
				if (lastGroundPos == FOOT_POS::None)
				{
					ground = FOOT_POS::Bottom;
				}
			}
			//�w�肵���ʒu�ɑ����z�u
			float stepPosY = stepPosition[(int)step];
			float groundPosY = groundPosition[(int)ground];

			stageWave[i].Initialize(resetPosX, stepPosY, groundPosY);

			lastGroundPos = ground;
			//���ꂪ�������J�E���g�𑝂₷
			resetCount++;
		}
	}
	//�P�O���������������ăJ�E���g�����Z�b�g����
	if (resetCount > 10)
	{
		speed += 10;
		for (int i = 0; i < WAVES; i++)
		{
			stageWave[i].SetSpeed(speed);
		}
		resetCount = 0;
	}
}

void WaveManager::CheckHitPlayer(Player* p)
{
	for (int i = 0; i < WAVES; i++)
	{
		stageWave[i].CheckHitPlayer(p);
	}
}

//�`��
void WaveManager::Render()
{
	for (int i = 0; i < WAVES; i++)
	{
		stageWave[i].Render();
	}
}

//���̑��x��n��
int WaveManager::GetSpeed()
{
	return speed;
}