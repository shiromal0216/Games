/*
	�쐬�ҁ@�F�@���ˁ@�_��
	�쐬���@�F�@2022/07/25
*/
#pragma once
#include "../ExtendLib/GameSystemObject/GameSystemObjects.h"
#include "Scenes/TitleScene.h"
#include "Scenes/PlayScene.h"
#include "Scenes/ResultScene.h"

/*
	�Q�[���̏������������邽�߂ɍ쐬����N���X�B
	�v���̃Q�[���v���O���}�[�́A���̃N���X�ƌĂ΂�鏑��������g���ăv���O�������쐬���Ă���B
*/
class Game
{
public:
	enum class SceneID
	{
		TitleScene,
		PlayScene,
		ResultScene,
		OverID
	};

private:
	//	������g�ލہA�����̊֐��Ŏg�p�������ϐ���錾����B
	//	Game::�֐���()�Ə����ꂽ�֐��Ȃ�A�ǂ��ł��g�p�\�B
	//	public:��艺�ł��ϐ��錾�ł��邪�A��{�I�Ɏw�����Ȃ���΁A
	//	�Q�[���v���O���~���O�̎��Ƃł�public:������ŕϐ��錾���邱�ƁI
	//	�܂��A��̊֐��̒��ł����g��Ȃ��ϐ��́A�֐��̒��Ő錾���邱�ƁI

	int screenWidth;
	int screenHeight;

	SceneID sceneID;

	TitleScene titleScene;
	PlayScene playScene;
	ResultScene resultScene;

	LONGLONG oldTime;
	LONGLONG nowTime;
	float deltaTime;

	bool firstWait;

	int countDown;	//�Q�[���J�n�O�̃J�E���g�_�E��

	GameSystemObjects* pGameSystem;

	//	�ϐ��錾�I���I
public:
	Game();									//	�R���X�g���N�^�ƌĂ΂��֐�
	~Game();								//	�f�X�g���N�^�ƌĂ΂��֐�

	void Initialize();						//	�Q�[���̏��������s���֐�
	void Update();							//	�Q�[���̍X�V�������s���֐�
	void Render();							//	�Q�[���̕`����s���֐��BDraw�Ə����ꍇ������
	void Finalize();						//	�Q�[���̏I���������s���֐��B����������K�v�ȏꍇ�iRelease�֐��Ȃǂ��Ăяo���j�́A���̊֐����ŏ�������

	void SetScreenSize(int width, int height);

	void SetGameSystemObjects(GameSystemObjects* pObj);

	//	�֐��錾�I���I
};

