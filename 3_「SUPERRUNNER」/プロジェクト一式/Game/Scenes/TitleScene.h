//------------------------------------------//
//	����ҁF���ˍ_��						//
//	�T�@�v�F�^�C�g���V�[���w�b�_�[			//
//------------------------------------------//

#pragma once
#include "../../ExtendLib/GameSystemObject/GameSystemObjects.h"

class TitleScene
{
	enum class PhaseType
	{
		RenderTitle,
		RenderPlay,

		OverID
	};

	typedef struct Vector
	{
		float x;
		float y;
	}Position;

private:
public:

private:
	GameSystemObjects* pGameSystem;
	bool	changeSceneFlag;
public:

private:

	PhaseType phaseID;
	int titleCount;
	int titleTextureID;
	int titlePushTextureID;

	Vector titleLogoSize;
	Vector pushSpaceSize;

	//	�ϐ��錾�I���I

public:
	TitleScene();							//	�R���X�g���N�^�ƌĂ΂��֐�
	~TitleScene();							//	�f�X�g���N�^�ƌĂ΂��֐�


	void Initialize(int screenWidth, int screenHeight);						//	�Q�[���̏��������s���֐�
	void Update(float deltaTime);					//	�Q�[���̍X�V�������s���֐�
	void Render();							//	�Q�[���̕`����s���֐��BDraw�Ə����ꍇ������
	void Finalize();						//	�Q�[���̏I���������s���֐��B����������K�v�ȏꍇ�iRelease�֐��Ȃǂ��Ăяo���j�́A���̊֐����ŏ�������

	void SetGameSystemObject(GameSystemObjects* pObj);
	bool GetChangeSceneFlag();
	//	�֐��錾�I���I
};
