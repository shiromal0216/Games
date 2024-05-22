#include "pch.h"
#include "StageSelectScene.h"
#include "GameStageScene.h"

using namespace DirectX;

StageSelectScene::StageSelectScene()
{
}

void StageSelectScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	// ���Ă���V�[�����I�[�v��������
	auto transitionMask = GetUserResources()->GetTransitionMask();
	transitionMask->Open();
}

void StageSelectScene::Update(float elapsedTime)
{
	// ���̃V�[����
	auto transitionMask = GetUserResources()->GetTransitionMask();
	auto kbTracker = GetUserResources()->GetKeyboardStateTracker();
	auto kb = kbTracker->GetLastState();

	// ------------------------------------- //
	// �㉺�L�[�Ńt���[�����ړ�
	// ------------------------------------- //
	if (kbTracker->pressed.Up && m_freamPos.y > m_freamMin)
	{
		m_freamPos.y -= 140.0f;
	}
	if (kbTracker->pressed.Down && m_freamPos.y < m_freamMax)
	{
		m_freamPos.y += 140.0f;
	}

	// �I�[�v�����Ă���Enter�L�[�������ꂽ
	if (transitionMask->IsOpen() && kbTracker->pressed.Enter && m_freamPos.y == m_freamMin)
	{
		// �N���[�Y����
		transitionMask->Close();
	}
	// �N���[�Y������ʂ̃V�[����
	if (transitionMask->IsClose() && transitionMask->IsEnd())
	{
		ChangeScene<GameStageScene>();
	}
}

void StageSelectScene::Render()
{
	m_spriteBatch->Begin();

	// �u�w�i�v�摜��`�悷��
	m_spriteBatch->Draw(m_backGroundSRV.Get(), SimpleMath::Vector2(BACKGROUND_POSITION));
	// �u�X�e�[�W�P�v�摜��`�悷��
	m_spriteBatch->Draw(m_stage1ButtonSRV.Get(), SimpleMath::Vector2(STAGE1_POSITION));
	// �u�X�e�[�W�Q�v�摜��`�悷��
	m_spriteBatch->Draw(m_stage2ButtonSRV.Get(), SimpleMath::Vector2(STAGE2_POSITION));
	// �u�X�e�[�W�R�v�摜��`�悷��
	m_spriteBatch->Draw(m_stage3ButtonSRV.Get(), SimpleMath::Vector2(STAGE3_POSITION));
	// �u�t���[���v�摜��`�悷��
	m_spriteBatch->Draw(m_freamSRV.Get(), SimpleMath::Vector2(FREAM_POSITION) + m_freamPos);
	// �u�I���ē��v�摜��`�悷��
	m_spriteBatch->Draw(m_selectButtonSRV.Get(), SimpleMath::Vector2(SELECT_POSITION));
	// �u�I������v�摜��`�悷��
	m_spriteBatch->Draw(m_selectEnterSRV.Get(), SimpleMath::Vector2(ENTER_POSITION));

	if (m_freamPos.y == m_freamMin) {
		// �u�X�e�[�W�P���e�v�摜��`�悷��
		m_spriteBatch->Draw(m_stage1ContSRV.Get(), SimpleMath::Vector2(STAGECONT_POSITION));
	}
	if (m_freamPos.y == m_freamMiddle){
		// �u�X�e�[�W�Q���e�v�摜��`�悷��
		m_spriteBatch->Draw(m_stage2ContSRV.Get(), SimpleMath::Vector2(STAGECONT_POSITION));
	}
	if (m_freamPos.y == m_freamMax) {
		// �u�X�e�[�W�R���e�v�摜��`�悷��
		m_spriteBatch->Draw(m_stage3ContSRV.Get(), SimpleMath::Vector2(STAGECONT_POSITION));
	}

	m_spriteBatch->End();
}

void StageSelectScene::Finalize()
{
	m_stage1ButtonSRV.Reset();
	m_stage2ButtonSRV.Reset();
	m_stage3ButtonSRV.Reset();
	m_stage1ContSRV.Reset();
	m_stage2ContSRV.Reset();
	m_stage3ContSRV.Reset();
	m_freamSRV.Reset();
	m_selectButtonSRV.Reset();
	m_selectEnterSRV.Reset();
	m_backGroundSRV.Reset();

	m_spriteBatch.reset();
}

void StageSelectScene::CreateDeviceDependentResources()
{
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();

	// �X�v���C�g�o�b�`�̍쐬
	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	// �u�X�e�[�W�P�v�摜�̓ǂݍ���
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/Stage1Button.dds", nullptr, m_stage1ButtonSRV.ReleaseAndGetAddressOf())
	);
	// �u�X�e�[�W�Q�v�摜�̓ǂݍ���
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/Stage2Button.dds", nullptr, m_stage2ButtonSRV.ReleaseAndGetAddressOf())
	);
	// �u�X�e�[�W�R�v�摜�̓ǂݍ���
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/Stage3Button.dds", nullptr, m_stage3ButtonSRV.ReleaseAndGetAddressOf())
	);
	// �u�X�e�[�W�P���e�v�摜�̓ǂݍ���
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/Stage1Contents.dds", nullptr, m_stage1ContSRV.ReleaseAndGetAddressOf())
	);
	// �u�X�e�[�W�Q���e�v�摜�̓ǂݍ���
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/Stage2Contents.dds", nullptr, m_stage2ContSRV.ReleaseAndGetAddressOf())
	);
	// �u�X�e�[�W�R���e�v�摜�̓ǂݍ���
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/Stage3Contents.dds", nullptr, m_stage3ContSRV.ReleaseAndGetAddressOf())
	);
	// �u�t���[���v�摜�̓ǂݍ���
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/SelectFream.dds", nullptr, m_freamSRV.ReleaseAndGetAddressOf())
	);
	// �u�I���ē��v�摜�̓ǂݍ���
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/SelectButton.dds", nullptr, m_selectButtonSRV.ReleaseAndGetAddressOf())
	);
	// �u�I������v�摜�̓ǂݍ���
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/SelectEnter.dds", nullptr, m_selectEnterSRV.ReleaseAndGetAddressOf())
	);
	// �u�w�i�v�摜�̓ǂݍ���
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/BackGround.dds", nullptr, m_backGroundSRV.ReleaseAndGetAddressOf())
	);
}

void StageSelectScene::CreateWindowSizeDependentResources()
{
}

void StageSelectScene::OnDeviceLost()
{
	Finalize();
}
