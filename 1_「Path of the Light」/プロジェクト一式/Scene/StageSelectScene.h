#pragma once

#include "ImaseLib/SceneManager.h"
#include "ImaseLib/UserResources.h"
#include "ImaseLib/ResourceManager.h"

class StageSelectScene : public Imase::Scene<UserResources>
{
public:
	
	// �R���X�g���N�^
	StageSelectScene();

	// ������
	void Initialize() override;

	// �X�V
	void Update(float elapsedTime) override;

	// �`��
	void Render() override;

	// �I������
	void Finalize() override;

	// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬����֐�
	void CreateDeviceDependentResources() override;

	// �E�C���h�E�T�C�Y�Ɉˑ����郊�\�[�X���쐬����֐�
	void CreateWindowSizeDependentResources() override;

	// �f�o�C�X���X�g�������ɌĂяo�����֐�
	void OnDeviceLost() override;

private:

	// �X�v���C�g�o�b�`�ւ̃|�C���^
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// �X�e�[�W�P�{�^���̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stage1ButtonSRV;
	// �X�e�[�W�Q�{�^���̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stage2ButtonSRV;
	// �X�e�[�W�R�{�^���̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stage3ButtonSRV;
	// �X�e�[�W�P���e�摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stage1ContSRV;
	// �X�e�[�W�Q���e�摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stage2ContSRV;
	// �X�e�[�W�R���e�摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stage3ContSRV;
	// �t���[���̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_freamSRV;
	// �I���ē��摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_selectButtonSRV;
	// �I������摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_selectEnterSRV;
	// �w�i�摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backGroundSRV;


private:

	// �t���[���̈ʒu
	DirectX::SimpleMath::Vector2 m_freamPos;

	// �t���[���̍ŏ��̈ʒu
	float m_freamMin = 0.0f;
	// �t���[���̒��Ԃ̈ʒu
	float m_freamMiddle = 140.0f;
	// �t���[���̍ő�̈ʒu
	float m_freamMax = 280.0f;

private:

	// �X�e�[�W�P�{�^���̃e�N�X�`���n���h���̈ʒu
	DirectX::SimpleMath::Vector2 STAGE1_POSITION = DirectX::SimpleMath::Vector2(150.0f, 175.0f);
	// �X�e�[�W�Q�{�^���̃e�N�X�`���n���h���̈ʒu
	DirectX::SimpleMath::Vector2 STAGE2_POSITION = DirectX::SimpleMath::Vector2(150.0f, 315.0f);
	// �X�e�[�W�R�{�^���̃e�N�X�`���n���h���̈ʒu
	DirectX::SimpleMath::Vector2 STAGE3_POSITION = DirectX::SimpleMath::Vector2(150.0f, 455.0f);
	// �X�e�[�W���e�̃e�N�X�`���n���h���̈ʒu
	DirectX::SimpleMath::Vector2 STAGECONT_POSITION = DirectX::SimpleMath::Vector2(700.0f, 100.0f);
	// �t���[���̃e�N�X�`���n���h���̈ʒu
	DirectX::SimpleMath::Vector2 FREAM_POSITION = DirectX::SimpleMath::Vector2(133.0f, 153.0f);
	// �I���ē��摜�̃e�N�X�`���n���h���̈ʒu
	DirectX::SimpleMath::Vector2 SELECT_POSITION = DirectX::SimpleMath::Vector2(50.0f, 600.0f);
	// �I������摜�̃e�N�X�`���n���h���̈ʒu
	DirectX::SimpleMath::Vector2 ENTER_POSITION = DirectX::SimpleMath::Vector2(420.0f, 600.0f);
	// �w�i�摜�̃e�N�X�`���n���h���̈ʒu
	DirectX::SimpleMath::Vector2 BACKGROUND_POSITION = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
};
