#pragma once

#include "ImaseLib/Camera.h"

// �Q�[���J����
class GameCamera : public Imase::Camera
{
public:

	// �J�����̃^�C�v
	enum class Type
	{
		Type_A,	// �v���C���[�̌�납��ǂ�������J����
		Type_B	// �v���C���[�̎�������J����
	};

private:

	// ���݂̃J�����̃^�C�v
	Type m_type;

	// ��]�p�i���W�A���j
	float m_angle;

	// �v���C���[�̈ʒu
	const DirectX::SimpleMath::Vector3* m_pPlayerPos;

	// �v���C���[�̉�]
	const DirectX::SimpleMath::Quaternion* m_pPlayerRotate;

public:

	// �R���X�g���N�^
	GameCamera();

	// �X�V�֐�
	void Update(float elapsedTime);

	// �v���C���[�̈ʒu�Ɖ�]��ݒ肷��֐�
	void SetPlayer(
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Quaternion& rotate
	)
	{
		m_pPlayerPos = &position;
		m_pPlayerRotate = &rotate;
	}

	// �J�����̃^�C�v�̐؂�ւ��֐�
	void SetType(Type type) { m_type = type; }

};
