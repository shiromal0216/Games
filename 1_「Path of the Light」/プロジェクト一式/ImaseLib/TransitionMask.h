//--------------------------------------------------------------------------------------
// File: TransitionMask.h
//
// ��ʐ؂�ւ��p�̃}�X�N�\���N���X
//
// Date: 2023.9.10
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

class TransitionMask
{
public:

	enum class CreateMaskRequest
	{
		NONE,	// ���N�G�X�g�Ȃ�
		COPY,	// �t���[���o�b�t�@�̓��e���R�s�[
	};

private:

	// ����(0�`1)
	float m_rate;

	// �I�[�v�����̓N���[�Y����܂ł̎���
	float m_interval;

	// �I�[�v���t���O�itrue�̏ꍇ�I�[�v���j
	bool m_open;

	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// �쐬���N�G�X�g
	CreateMaskRequest m_request;

	// �s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS_Fade;

	// �萔�o�b�t�@�̍\����
	struct ConstantBuffer
	{
		float rate;		// �����i0�`1�j
		float pad[3];
	};

	// �萔�o�b�t�@�ւ̃|�C���^
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

	// �}�X�N�p�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_maskTexture;

public:

	// �R���X�g���N�^
	TransitionMask(
		ID3D11Device* device,
		ID3D11DeviceContext* context,
		float interval = 1.0f
	);

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Draw(
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		ID3D11ShaderResourceView* texture,
		const RECT& rect
	);

	// �I�[�v������֐�
	void Open();

	// �N���[�Y����֐�
	void Close();

	// ��Ԃ��m�F����֐�
	bool IsOpen() const { return m_open; }
	bool IsClose() const { return !m_open; }

	// �����i0�`1�j���擾����֐�
	float GetRate() const
	{
		if (m_open) return 1.0f - m_rate;
		return m_rate;
	}

	// �������Ă��邩�m�F����֐�
	bool IsEnd() const
	{
		if (GetRate() == 1.0f) return true;
		return false;
	}

	// �I�[�v���A�N���[�Y�܂ł̎��Ԃ�ݒ肷��֐�
	void SetInterval(float interval) { m_interval = interval; }

	// �}�X�N�쐬���N�G�X�g�̎擾
	CreateMaskRequest GetCreateMaskRequest() const { return m_request; }

	// �}�X�N�쐬���N�G�X�g�̐ݒ�
	void SetCreateMaskRequest(CreateMaskRequest request) { m_request = request; }

};
