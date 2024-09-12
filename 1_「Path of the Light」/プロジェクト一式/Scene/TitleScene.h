//================================================
// �T�@�v�F�^�C�g���V�[���̃v���O����
// �쐬���F2024/4/22
// �쐬�ҁF�}�c�h �R�E�L
//================================================

#pragma once

#include "ImaseLib/SceneManager.h"
#include "ImaseLib/UserResources.h"
#include "ImaseLib/DebugCamera.h"
#include "ImaseLib/DisplayCollision.h"
#include "Object.h"
#include "ImaseLib/ModelCollision.h"
#include "GameCamera.h"
#include "ImaseLib/ModelPart.h"
#include "RenderTexture.h"
#include "ImaseLib/DepthStencil.h"

class TitleScene : public Imase::Scene<UserResources>
{
public:
	
	// �R���X�g���N�^
	TitleScene();

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

	// �f�o�b�O�J�����ւ̃|�C���^
	std::unique_ptr<Imase::DebugCamera> m_debugCamera;

	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;

	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_proj;

private:
	// ���{�b�g�̈ʒu
	DirectX::SimpleMath::Vector3 m_robotPosition;

	// ���{�b�g�̉�]
	DirectX::SimpleMath::Quaternion m_robotRotate;

	// ���{�b�g�̍��r�̈ʒu
	DirectX::SimpleMath::Vector3 m_leftArmPosition;

	// �J����
	GameCamera m_camera;

	// ���{�b�g�̊e�p�[�c���f���ւ̃|�C���^
	std::unique_ptr <DirectX::Model> m_headModel;
	std::unique_ptr <DirectX::Model> m_bodyModel;
	std::unique_ptr <DirectX::Model> m_legModel;
	std::unique_ptr <DirectX::Model> m_armRModel;
	std::unique_ptr <DirectX::Model> m_armLModel;
	std::unique_ptr <DirectX::Model> m_rightModel;
	std::unique_ptr <DirectX::Model> m_missileModel;

	enum { ROOT, HEAD, BODY, LEG, ARM_R, ARM_L, RIGHT, MISSILE, PARTS_CNT };

	// ���{�b�g�̃p�[�c�ւ̃|�C���^
	std::unique_ptr<Matsudo::ModelPart> m_parts[PARTS_CNT];

private:

	// �����f���ւ̃|�C���^
	std::unique_ptr<DirectX::Model> m_floorModel;

	// �e�̃e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shadowTexture;

	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatch;

	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// �e�̏������֐�
	void InitializeShadow(ID3D11Device* device, ID3D11DeviceContext* context);

	// �e�̏I������
	void ResetShadow();

	// �e�̕`��֐�
	void DrawShadow(
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		DirectX::SimpleMath::Vector3 position,
		float radius = 0.5f
	);

	// �[�x�X�e���V���X�e�[�g�i���p�j
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState_Floor;

	// �[�x�X�e���V���X�e�[�g�i�e�p�j
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState_Shadow;

private:

	// �����_�[�e�N�X�`��(�V�[���S��)
	std::unique_ptr<DX::RenderTexture> m_offscreenRT;

	// �����_�[�e�N�X�`��(�u���[���p)
	std::unique_ptr<DX::RenderTexture> m_blur1RT;
	std::unique_ptr<DX::RenderTexture> m_blur2RT;

	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// �x�[�V�b�N�|�X�g�v���Z�X
	std::unique_ptr<DirectX::BasicPostProcess> m_basicPostProcess;

	// �f���A���|�X�g�v���Z�X
	std::unique_ptr<DirectX::DualPostProcess> m_dualPostProcess;

private:
	
	// �V���h�E�}�b�v�̃T�C�Y
	static const int SHADOWMAP_SIZE = 512;

	// �V���h�E�}�b�v�p�i�����_�[�e�N�X�`���j
	std::unique_ptr<DX::RenderTexture> m_shadowMapRT;

	// �V���h�E�}�b�v�p�i�f�v�X�X�e���V���j
	std::unique_ptr<Matsudo::DepthStencil> m_shadowMapDS;

	// ���_�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS_Depth;

	// �s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS_Depth;

	// ���C�g�̈ʒu
	DirectX::SimpleMath::Vector3 m_lightPosition;

	// ���C�g�̉�]
	DirectX::SimpleMath::Quaternion m_lightRotate;

private:

	// �萔�o�b�t�@�̍\����
	struct ConstantBuffer
	{
		DirectX::XMMATRIX lightViewProj;	// ���C�g�̓��e��Ԃ֍��W�ϊ�����s��
		DirectX::XMVECTOR lightPosition;	// ���C�g�̈ʒu
		DirectX::XMVECTOR lightDirection;	// ���C�g�̕���
		DirectX::XMVECTOR lightAmbient;		// ���C�g�̊���
	};

	// �萔�o�b�t�@�ւ̃|�C���^
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

	// �萔�o�b�t�@�̍\���́i�p�ɂɍX�V����Ȃ����j
	struct ConstantBuffer2
	{
		float fCosTheta;		// �X�|�b�g���C�g��fov/2
		float pad[3];
	};

	// �萔�o�b�t�@�ւ̃|�C���^
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer2;

	// ���_�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS;

	// �s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS;

	// �T���v���[
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_shadowMapSampler;

	// �X�|�b�g���C�g�̃��f���ւ̃|�C���^
	std::unique_ptr<DirectX::Model> m_spotLightModel;

	// �X�|�b�g���C�g�͈̔͂̊p�x
	float m_lightTheta;

private:

	// �^�C�g�����S�摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_titleSRV;

	// Press to start�摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_startSRV;

	// ------------------------------------------------------- //
	// �摜�̈ʒu�̒萔��
	// ------------------------------------------------------- //
	// �^�C�g�����S�摜�̈ʒu
	const DirectX::SimpleMath::Vector2 TITLE_POSITION = { 60.0f, 50.0f };
	// Press to start�摜�̃e�N�X�`���n���h��
	const DirectX::SimpleMath::Vector2 START_POSITION = { 400.0f, 500.0f };

};

