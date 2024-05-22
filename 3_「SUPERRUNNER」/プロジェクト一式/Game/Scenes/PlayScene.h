//------------------------------------------//
//	����ҁF���ˍ_��						//
//	�T�@�v�F�v���C�V�[���w�b�_�[			//
//------------------------------------------//

#pragma once
#include "../../ExtendLib/GameSystemObject/GameSystemObjects.h"

#include "../GameObjects/PlayScene/WaveManager.h"
#include "../GameObjects/PlayScene/Player.h"
#include"../GameObjects/PlayScene/BackGround.h"

class PlayScene
{
private:
	GameSystemObjects* pGameSystem;
	bool	changeSceneFlag;
	Player player;

	WaveManager waveManager;

	BackGround backGround;

	int meteo;	// ���e�I
	int score;	// �X�R�A

	bool ret;
	bool movemeteo;	// ���e�I�̓�������
	int meteoPositionX;	// ���e�I��X���̈ʒu
	int meteoPositionY;	// ���e�I��Y���̈ʒu

	int bgmID;			// BGM

	int countDown;		// �J�E���g�_�E��

public:
	PlayScene();												//	�R���X�g���N�^�ƌĂ΂��֐�
	~PlayScene();												//	�f�X�g���N�^�ƌĂ΂��֐�


	void Initialize(int screenWidth, int screenHeight);			//	�Q�[���̏��������s���֐�
	void Update(float deltaTime);								//	�Q�[���̍X�V�������s���֐�
	void Render();												//	�Q�[���̕`����s���֐�
	void Finalize();											//	�Q�[���̏I���������s���֐�

	void SetGameSystemObject(GameSystemObjects* pObj);
	bool GetChangeSceneFlag();

	bool CheckHitPlayer();										// �v���C���[�ƃ��e�I�̓����蔻��

	int GetScore();												// �X�R�A���Q�b�g���邽�߂̊֐�
};
