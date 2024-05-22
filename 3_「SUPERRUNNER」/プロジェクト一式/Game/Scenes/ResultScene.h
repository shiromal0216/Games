//------------------------------------------//
//	����ҁF���ˍ_��						//
//	�T�@�v�F���U���g�V�[���w�b�_�[			//
//------------------------------------------//

#pragma once
#include "../../ExtendLib/GameSystemObject/GameSystemObjects.h"

class ResultScene
{
private:
	GameSystemObjects* pGameSystem;
	bool	changeSceneFlag;

	long haveScore;

public:
	ResultScene();												//	�R���X�g���N�^�ƌĂ΂��֐�
	~ResultScene();												//	�f�X�g���N�^�ƌĂ΂��֐�


	void Initialize(int screenWidth, int screenHeight);			//	�Q�[���̏��������s���֐�
	void Update(float deltaTime);								//	�Q�[���̍X�V�������s���֐�
	void Render();												//	�Q�[���̕`����s���֐�
	void Finalize();											//	�Q�[���̏I���������s���֐�

	void SetGameSystemObject(GameSystemObjects* pObj);
	void SetScore(int score);
	bool GetChangeSceneFlag();
};
