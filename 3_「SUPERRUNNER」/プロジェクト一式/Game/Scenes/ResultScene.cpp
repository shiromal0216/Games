//------------------------------------------//
//	����ҁF���ˍ_��						//
//	�T�@�v�F���U���g�V�[��					//
//------------------------------------------//

#include <DxLib.h>
#include "ResultScene.h"

ResultScene::ResultScene() :
	haveScore(0),
	pGameSystem(nullptr),
	changeSceneFlag(false)
{

}

ResultScene::~ResultScene()
{

}

void ResultScene::Initialize(int screenWidth, int screenHeigh)
{
	changeSceneFlag = false;
}

void ResultScene::Update(float deltaTime)
{
	//�X�y�[�X�L�[��������Ă�����A�t�F�[�h�A�E�g���s��
	if (pGameSystem->GetInputSystem().IsKeyPressed(KEY_INPUT_SPACE))
	{
		changeSceneFlag = true;
	}
}

void ResultScene::Render()
{
	int defaultFontSize = GetFontSize();

	// ���U���g�V�[���̕����\��
	SetFontSize(54);
	DrawFormatString(170, 180, GetColor(255, 255, 255), "�i�񂾋����F");
	DrawFormatString(480, 180, GetColor(255, 255, 255), "%dm", haveScore);
	DrawFormatString(130, 300, GetColor(255, 255, 255), "�]���F");
	DrawFormatString(130, 400, GetColor(255, 255, 255), "�X�y�[�X�Ń^�C�g����");

	// �i�񂾋����ɂ���ĕ]����ς���
	if (haveScore < 500)
	{
		DrawString(320, 300, "�܂��܂����ˁI", GetColor(255, 255, 255), 1);
	}
	else if(haveScore < 1999)
	{
		DrawString(320, 300, "�Ȃ��Ȃ����ˁI", GetColor(255, 255, 255), 1);
	}
	else
	{
		DrawString(320, 300, "�f���炵���I", GetColor(255, 255, 255), 1);
	}

	SetFontSize(defaultFontSize);
}

void ResultScene::Finalize()
{
	
}

void ResultScene::SetGameSystemObject(GameSystemObjects* pObj)
{
	pGameSystem = pObj;
}

bool ResultScene::GetChangeSceneFlag()
{
	// �t���O��Ԃ�
	return changeSceneFlag;
}

void ResultScene::SetScore(int score)
{
	// �X�R�A���Z�b�g����
	haveScore = score;
}