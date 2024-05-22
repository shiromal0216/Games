#pragma once

#include "../ImaseLib/TaskManager.h"
#include "../SpriteSheet.h"

// ������\������^�X�N
class Number : public Imase::Task
{
public:

	// �R���X�g���N�^
	Number(
		std::unique_ptr<DirectX::SpriteBatch>* pSpriteBatch,
		ID3D11ShaderResourceView** pNumberSRV,
		uint32_t digit = 8,	// �����i�\���ʒu���牽���ŕ\�����邩�w�肷��B�E�����̂��߁j
		float scale = 1.0f
	);

	// �X�V
	bool Update(float elapsedTime) override;

	// �`��
	void Render() override;

private:

	// �X�v���C�g�V�[�g
	std::unique_ptr<SpriteSheet> m_sprites;

	// �X�v���C�g�o�b�`�ւ̃|�C���^
	std::unique_ptr<DirectX::SpriteBatch>* m_pSpriteBatch;

	// �����̃e�N�X�`���n���h���ւ̃|�C���^
	ID3D11ShaderResourceView** m_pNumberSRV;

	// �\���ʒu
	DirectX::SimpleMath::Vector2 m_pos;

	// �\������
	uint32_t m_digit;

	// �X�P�[��
	float m_scale;

	// �\�����鐔��
	uint32_t m_number;

	// �O�`�X�̐����̃X�v���C�g�t���[���ւ̃|�C���^
	const SpriteSheet::SpriteFrame* m_frame[10];

public:

	// �\��������ݒ肷��֐�
	void SetDigit(uint32_t digit) { m_digit = digit; }

	// �X�P�[����ݒ肷��֐�
	void SetScale(float scale) { m_scale = scale; }

	// �\�����鐔����ݒ肷��֐�
	void SetNumber(uint32_t number) { m_number = number; }

	// �\���ʒu��ݒ��ݒ肷��֐�
	void SetPosition(DirectX::SimpleMath::Vector2 pos) { m_pos = pos; }

};
