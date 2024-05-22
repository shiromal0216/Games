//------------------------------------------//
//	����ҁF���ˍ_��						//
//	�T�@�v�F�v���C���[�̃v���W�F�N�g		//
//------------------------------------------//

#include <DxLib.h>

#include "Player.h"
#include <math.h>
#include <float.h>

//�v���C���[�̏����ʒu
#define START_POSITION_X	(200)
#define START_POSITION_Y	(500)

//��ʂ̕�
#define SCREEN_HEIGHT	(800)

//�v���C���[�̉摜�̂P�L�������̑傫��
#define CHIP_WIDTH	(32)
#define CHIP_HEIGHT	(32)

//�v���C���[�̕\���g�嗦
#define PLAYER_WIDTH_RATE	(1.5f)
#define PLAYER_HEIGHT_RATE	(1.5f)

//��ʏ�̃v���C���[�̃T�C�Y
#define PLAYER_WIDTH (int)(CHIP_WIDTH * PLAYER_WIDTH_RATE)
#define PLAYER_HEIGHT (int)(CHIP_HEIGHT * PLAYER_HEIGHT_RATE)

//�摜�̒��S�ʒu
#define ANCHOR_X (CHIP_WIDTH / 2)
#define ANCHOR_Y (CHIP_HEIGHT)

//�~����
#define PI					(3.14159265358979323846)

//�R���X�g���N�^
Player::Player():
	position{0.0f,0.0f},
	velocity{ 0.0f,0.0f },
	acceleration(0.0f),
	gravity(0.0f),
	jumpNum(0),
	jumpRot(0),
	pSystemObject(nullptr),
	animationKey(1),
	isAlive(true)
{
	// �摜�ǂݍ���
	textureHandle = LoadGraph("Resources/Textures/Character.png");
}

//�f�X�g���N�^
Player::~Player()
{
}

//������
void Player::Initialize()
{
	position.x = START_POSITION_X;
	position.y = START_POSITION_Y;

	velocity.x = 0.0f;
	velocity.y = 0.0f;

	acceleration = 0.0f;

	gravity = 0.0f;

	jumpNum = MAX_JUMP;

	jumpRot = 0;

	animationKey = 1;

	isAlive = true;
}

Position Player::GetPosition()
{
	return position;
}
float Player::GetVelocityY()
{
	return velocity.y + gravity;
}

//�X�V
void Player::Update()
{
	float x = 0.0f;
	float y = 0.0f;

	velocity.x = 0.0f;

	// �v���C���[�̏����ʒu
	if (position.x < START_POSITION_X)
	{
		acceleration = (START_POSITION_X - position.x) * 0.01f;
		velocity.x = 1.0f;

		if (acceleration > 1.0f)
		{
			acceleration = 1.0f;
		}
	}
	//�ȈՖ��C����
	if (fabsf(velocity.x) < FLT_EPSILON)
	{
		acceleration *= 0.8f;
	}

	//��L�[���͂ŃW�����v
	if (pSystemObject->GetInputSystem().IsKeyPressed(KEY_INPUT_UP) == TRUE && jumpNum > 0)
	{
		velocity.y = 6.0f;
		gravity = 0.0f;

		//�W�����v�񐔂̏���
		jumpNum--;
	}

	//���x�{�����x�̃x�N�g�������
	x = velocity.x + acceleration;

	//�d�͉����x��ݒ�
	if (pSystemObject->GetInputSystem().IsKeyDown(KEY_INPUT_UP) == TRUE)
	{
		gravity -= 0.1f;
	}
	else
	{
		gravity -= 0.3f;
	}
	//�W�����v�x�N�g�������
	y = velocity.y + gravity;

	//Y�x�N�g���𔽓]
	y *= -1.0f;

	//���W�Ƀx�N�g�������Z
	position.x += x;
	position.y += y;

	if (position.y < PLAYER_HEIGHT)
	{
		position.y = PLAYER_HEIGHT;
	}

	// ���ʔ����Ԃ�
	if (position.y > SCREEN_HEIGHT + PLAYER_WIDTH)
	{
		isAlive = false;
	}
}

//�`��
void Player::Render()
{
	int interval = 10;

	int keys[4] = { 0,1,2,1 };

	if (jumpNum == 0)
	{
		//�Q�i�ڂ̃W�����v�͉�]����
		jumpRot += 10;
		DrawRectRotaGraph3(
			(int)position.x,
			(int)position.y - (PLAYER_HEIGHT / 2),
			0,
			CHIP_HEIGHT * 2,
			CHIP_WIDTH,
			CHIP_HEIGHT,
			ANCHOR_X,
			ANCHOR_Y - (CHIP_HEIGHT / 2),
			PLAYER_HEIGHT_RATE,
			PLAYER_WIDTH_RATE,
			jumpRot * PI / 180.0,
			textureHandle,
			TRUE);
	}
	else if (jumpNum == 1)
	{
		DrawRectRotaGraph3(
			(int)position.x,
			(int)position.y,
			0,
			CHIP_HEIGHT * 2,
			CHIP_WIDTH,
			CHIP_HEIGHT,
			ANCHOR_X,
			ANCHOR_Y,
			1.5,
			1.5,
			0.0,
			textureHandle,
			TRUE);
	}
	else
	{
		//�n�ʂɂ���Ƃ��͘r��U��
		DrawRectRotaGraph3(
			(int)position.x,
			(int)position.y,
			CHIP_WIDTH * keys[animationKey / interval],
			CHIP_HEIGHT * 2,
			CHIP_WIDTH,
			CHIP_HEIGHT,
			ANCHOR_X,
			ANCHOR_Y,
			1.5,
			1.5,
			0.0,
			textureHandle,
			TRUE);
		jumpRot = 0;
		animationKey = (animationKey + 1) % (4 * interval);
	}
	
}

void Player::SetSystemObject(GameSystemObjects* pObj)
{
	pSystemObject = pObj;
}

void Player::ResetJumpCount()
{
	//�W�����v�񐔂̃��Z�b�g
	jumpNum = MAX_JUMP;
}

void Player::ResetJump()
{
	//�d�͂ƃW�����v�͂̃��Z�b�g
	velocity.y = 0.0f;
	gravity = 0.0f;
}

void Player::SetPosition(Position pos)
{
	position = pos;
}

void Player::Finalize()
{
	DeleteGraph(textureHandle);
}

// �������Ă��邩�ǂ���
bool Player::IsAlive()
{
	return isAlive;
}