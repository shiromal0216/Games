//------------------------------------------//
//	����ҁF���ˍ_��						//
//	�T�@�v�F�^�C�g���V�[��					//
//------------------------------------------//

#include "TitleScene.h"
#include <DxLib.h>

#define SCREEN_WIDTH		800	//��ʂ̉��T�C�Y
#define SCREEN_HEIGHT		600	//��ʂ̏c�T�C�Y

#define COUNTDOWN_NUM		60	//�_�ł̊Ԋu���ԁi1�b�j

TitleScene::TitleScene() :
	pGameSystem(nullptr),
	changeSceneFlag(false)
{
	//�^�C�g���ƃv���C�̃t�F�[�Y�̓��A�^�C�g���\����ݒ肷��
	phaseID = PhaseType::RenderTitle;
	//PushKey�̕\���_�ŗp�J�E���^�[
	titleCount = 0;

	//�^�C�g���摜��PushKey�̉摜�ǂݍ���
	titleTextureID = LoadGraph("Resources/Textures/Title.png");
	titlePushTextureID = LoadGraph("Resources/Textures/PushSpaceKey.png");
	
	//�摜�T�C�Y�����ꂼ��v��
	int sizeX, sizeY;

	// �^�C�g���摜��X,Y���̈ʒu-------------------------
	GetGraphSize(titleTextureID, &sizeX, &sizeY);

	titleLogoSize.x = (float)sizeX;
	titleLogoSize.y = (float)sizeY;

	GetGraphSize(titlePushTextureID, &sizeX, &sizeY);

	pushSpaceSize.x = (float)sizeX;
	pushSpaceSize.y = (float)sizeY;
	//---------------------------------------------------
}

TitleScene::~TitleScene()
{

}

void TitleScene::Initialize(int screenWidth, int screenHeigh)
{
	changeSceneFlag = false;
}

void TitleScene::Update(float deltaTime)
{
	// �X�y�[�X�L�[����������v���C�V�[���ֈړ�
	if (pGameSystem->GetInputSystem().IsKeyPressed(KEY_INPUT_SPACE))
		changeSceneFlag = true;
	// �X�y�[�X�L�[�摜��_�ł�����
	titleCount++;
	if (titleCount > COUNTDOWN_NUM)
	{
		titleCount = 0;
	}
}

void TitleScene::Render()
{
	//�^�C�g�����S�̕\��
	DrawGraph(0, 0, titleTextureID, 1);

	if (titleCount >= 30)
	{
		//	30�`59�t���[���ł���΁APushKey��\��
		DrawExtendGraph(
			(int)(SCREEN_WIDTH / 2.0f - SCREEN_WIDTH / 4.0f), (int)(SCREEN_HEIGHT / 2.0f + 100),
			(int)(SCREEN_WIDTH / 2.0f + SCREEN_WIDTH / 4.0f), (int)(SCREEN_HEIGHT / 2.0f + 150),
			titlePushTextureID, 1);
	}
}

void TitleScene::Finalize()
{
	//	�ǂݍ��񂾉摜�̔j��
	DeleteGraph(titleTextureID);
	DeleteGraph(titlePushTextureID);

	titleTextureID = 0;
	titleTextureID = 0;
}

void TitleScene::SetGameSystemObject(GameSystemObjects* pObj)
{
	pGameSystem = pObj;
}

bool TitleScene::GetChangeSceneFlag()
{
	return changeSceneFlag;
}
