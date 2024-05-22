#include "TitleScene.h"
#include <DxLib.h>

#define SCREEN_WIDTH	800//��ʉ��T�C�Y
#define SCREEN_HEIGHT	600//��ʏc�T�C�Y

TitleScene::TitleScene() :
	pGameSystem(nullptr),
	mouseInputFlag(true),
	changeSceneFlag(false),
	backcolor(0)
{
	//�^�C�g���ƃv���C�̃t�F�[�Y�̓��A�^�C�g���\����ݒ肷��
	phaseID = PhaseType::RenderTitle;
	//Title��PushKey�̕\���_�ŗp�J�E���^�[
	titleCount = 0;

	//�^�C�g���摜��PushKey�̉摜�ǂݍ���
	titleTextureID = LoadGraph("Resources/Textures/Title.png");
	titleTextureID2 = LoadGraph("Resources/Textures/Title2.png");
	titlePushTextureID = LoadGraph("Resources/Textures/Click The Screen.png");

	//�w�i�摜�̓ǂݍ���
	backcolor = LoadGraph("Resources/Textures/haikei.png");

	//�摜�T�C�Y�����ꂼ��v��
	int sizeX, sizeY;

	GetGraphSize(titleTextureID, &sizeX, &sizeY);

	titleLogoSize.x = (float)sizeX;
	titleLogoSize.y = (float)sizeY;

	GetGraphSize(titleTextureID2, &sizeX, &sizeY);

	titleLogoSize.x = (float)sizeX;
	titleLogoSize.y = (float)sizeY;

	GetGraphSize(titlePushTextureID, &sizeX, &sizeY);

	pushSpaceSize.x = (float)sizeX;
	pushSpaceSize.y = (float)sizeY;
}

TitleScene::~TitleScene()
{

}

void TitleScene::Initialize(int screenWidth, int screenHeight)
{
	changeSceneFlag = false;
	mouseInputFlag = true;

	mFade = 0;
	mFadeFlag = false;
}

void TitleScene::Update(float deltaTime)
{
	int mouseX, mouseY;

	//�}�E�X�J�[�\���̍��W���擾
	GetMousePoint(&mouseX, &mouseY);

	//�L�[��������Ă���Ȃ�
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		if (!mouseInputFlag)
		{
			mFadeFlag = true;
		}
	}
	else
	{
		mouseInputFlag = false;
	}
	titleCount++;
	if (titleCount > 60)
	{
		titleCount = 0;
	}

	if (mFadeFlag == true)
	{
		mFade += 5;
	}

	if (mFade == 255)
	{
		mFade = 0;
		mFadeFlag = false;
		changeSceneFlag = true;
	}
}

void TitleScene::Render()
{
	//�w�i�摜�`��
	DrawGraph(0, 0, backcolor, 1);

	//�^�C�g���摜����ւ�
	DrawGraph(0, 0, titleTextureID, 1);

	if (titleCount >= 30)
	{
		DrawGraph(0, 0, titleTextureID2, 1);
	}

	//�^�C�g�����S�̕\��

	if (titleCount >= 30)
	{
		DrawExtendGraph(
			(int)(SCREEN_WIDTH / 2.0f - SCREEN_WIDTH / 4.0f), (int)(SCREEN_HEIGHT / 2.0f + 100),
			(int)(SCREEN_WIDTH / 2.0f + SCREEN_WIDTH / 4.0f), (int)(SCREEN_HEIGHT / 2.0f + 150),
			titlePushTextureID, 1);
	}
	
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, mFade);
	DrawBoxAA(0, 0, 1000, 1000, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0.0);
}

void TitleScene::Finalize()
{
	//�ǂݍ��񂾉摜�̔j��
	DeleteGraph(titleTextureID);
	DeleteGraph(titleTextureID2);
	DeleteGraph(titlePushTextureID);
	DeleteGraph(backcolor);

	titleTextureID = 0;
	titleTextureID2 = 0;
}

void TitleScene::SetGameSystemObject(GameSystemObjects* pObj)
{
	pGameSystem = pObj;
}

bool TitleScene::GetChangeSceneFlag()
{
	return changeSceneFlag;
}