#include "pch.h"
#include "GameCamera.h"

using namespace DirectX;

// �R���X�g���N�^
GameCamera::GameCamera()
	: m_type(Type::Type_A)
	, m_angle(0.0f)
	, m_pPlayerPos{}
	, m_pPlayerRotate{}
{
}

// �X�V�֐�
void GameCamera::Update(float elapsedTime)
{
	switch (m_type)
	{
	case Type::Type_A:	// �v���C���[�̌�납��ǂ�������J����
		{
			SimpleMath::Vector3 targetPos =
				*m_pPlayerPos + SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
			SimpleMath::Vector3 eyePos(0.0f, 0.5f, -2.0f);
			eyePos = SimpleMath::Vector3::Transform(eyePos, *m_pPlayerRotate);
			SetPositionTarget(targetPos + eyePos, targetPos);
		}
		break;
	case Type::Type_B:	// �v���C���[�̎�������J����
		{
			m_angle += XMConvertToRadians(30.0f) * elapsedTime;
			SimpleMath::Vector3 targetPos =
				*m_pPlayerPos + SimpleMath::Vector3(0.0f, 0.3f, 0.0f);
			SimpleMath::Vector3 eyePos(0.0f, 1.0f, 3.0f);
			eyePos = SimpleMath::Vector3::Transform(
				eyePos, SimpleMath::Matrix::CreateRotationY(m_angle));
			SetPositionTarget(targetPos + eyePos, targetPos);
		}
		break;
	default:
		break;
	}
}