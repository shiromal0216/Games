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

class GameStageScene : public Imase::Scene<UserResources>
{
public:
	
	// �R���X�g���N�^
	GameStageScene();

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

	// �Փ˔���̓o�^�֐�
	//void CollisionRegist();

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

	// ���{�b�g�̏㔼�g�̉�]
	DirectX::SimpleMath::Quaternion m_bodyRotate;

	// ���{�b�g�̍��r�̉�]
	DirectX::SimpleMath::Quaternion m_armLRotate;

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
	std::unique_ptr <DirectX::Model> m_lightModel;
	std::unique_ptr <DirectX::Model> m_missileModel;

	enum { ROOT, HEAD, BODY, LEG, ARM_R, ARM_L, LIGHT, MISSILE, PARTS_CNT };

	// ���{�b�g�̃p�[�c�ւ̃|�C���^
	std::unique_ptr<Imase::ModelPart> m_parts[PARTS_CNT];

	// �~�T�C�����˃t���O
	bool m_fireFlag;

	// �~�T�C���̈ʒu
	DirectX::SimpleMath::Vector3 m_missilePosition;

	// �~�T�C���̉�]
	DirectX::SimpleMath::Quaternion m_missileRotate;

	// �~�T�C���̈ړ�����
	float m_distance;

	// ���̋ʎ擾�t���O
	bool m_lightBallFlag;

	// �u�����Ȃ��v�摜�\���t���O
	bool m_nothingFlag;

	// �u���̋ʊl���v�摜�\���t���O
	bool m_lightgetFlag;

	// �u���l���v�摜�\���t���O
	bool m_keygetFlag;

	// �u�����������Ă���v�摜�\���t���O
	bool m_lockFlag;

	// �u���v�摜�\���t���O
	bool m_keyFlag;

	// �u�o�[���v�摜�\���t���O
	bool m_crowbarFlag;

	// �u�o�[���l���v�摜�\���t���O
	bool m_bargetFlag;

	// �u�����N���Ȃ��v�摜�\���t���O
	bool m_notgetFlag;

	// �u�����|�����Ă��ĊJ���Ȃ��v�摜�\���t���O
	bool m_stuckFlag;

	// �u�o�[���ł����J�����v�摜�\���t���O
	bool m_barOpenFlag;

	// �u���ŊJ�����v�摜�\���t���O
	bool m_keyOpenFlag;

	// �u�n�}�v�摜�\���t���O
	bool m_mapFlag;

	// �u�Q�[���ē��v�摜�\���t���O
	bool m_guideFlag;

	// �u�X�e�[�W�N���A�v�摜�\���t���O
	bool m_stageClearFlag;

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
	// �󔠃��f���ւ̃|�C���^
	std::unique_ptr<DirectX::Model> m_treasureModel;
	// WEC�ؔ����f���ւ̃|�C���^
	std::unique_ptr<DirectX::Model> m_WECBoxModel;
	// �ؔ����f���ւ̃|�C���^
	std::unique_ptr<DirectX::Model> m_woodBoxModel;
	// �˒I���f���ւ̃|�C���^
	std::unique_ptr<DirectX::Model> m_cabinetModel;
	// ���A�����f���ւ̃|�C���^
	std::unique_ptr<DirectX::Model> m_pottedModel;
	// �ǃ��f���ւ̃|�C���^
	std::unique_ptr<DirectX::Model> m_wallModel;
	// �x�b�h���f���ւ̃|�C���^
	std::unique_ptr<DirectX::Model> m_bedModel;
	// �����^�����f���ւ̃|�C���^
	std::unique_ptr<DirectX::Model> m_lanternModel;
	// �����f���ւ̃|�C���^
	std::unique_ptr<DirectX::Model> m_deskModel;

private:

	// �u���{�b�g�̈ړ��v�̐����摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_robotMoveSRV;
	// �u���{�b�g�̏㔼�g�̉�]�v�̐����摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_upbodySRV;
	// �u���r(���C�g)�̉�]�v�̐����摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ArmLSRV;
	// �uE���ׂ�v�摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_eCheckSRV;
	// �u���̋ʁv�摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_lightBallSRV;
	// �u�����Ȃ��v�摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_nothingSRV;
	// �u���̋ʊl���v�摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_lightgetSRV;
	// �u���l���v�摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_keygetSRV;
	// �u���v�摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_keySRV;
	// �u�����������Ă���v�摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_lockSRV;
	// �uE�͂߂�v�摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_eSetSRV;
	// �u�X�e�[�W�N���A�v�摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stageClearSRV;
	// �u�����N���Ȃ��v�摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_notgetSRV;
	// �u�Q�[���̈ē��v�摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_guideSRV;
	// �u�A�C�e���t���[���v�摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_itemFrameSRV;
	// �u�o�[���v�摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_crowBarSRV;
	// �u�o�[���l���v�摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_bargetSRV;
	// �u�����|�����ĊJ���Ȃ��v�摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stuckSRV;
	// �u�o�[���ł����J�����v�摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_barOpenSRV;
	// �u���ŊJ�����v�摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_keyOpenSRV;
	// �u�n�}�v�摜�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_mapSRV;

private:

	// �V���h�E�}�b�v�̃T�C�Y
	static const int SHADOWMAP_SIZE = 512;

	// ------------------------------------- //
	// ���f���̈ʒu�̒萔��
	// ------------------------------------- //
	// �󔠂̈ʒu
	const DirectX::SimpleMath::Vector3 TREASURE_POSITION = { 3.0f, 0.0f, 4.0f };
	// WEC�ؔ��̈ʒu
	const DirectX::SimpleMath::Vector3 WECBOX_POSITION = { 3.0f, 0.0f, 2.0f };
	// �ؔ��̈ʒu
	const DirectX::SimpleMath::Vector3 WOODBOX_POSITION = { 3.0f, 0.0f, 0.0f };
	// �˒I�̈ʒu
	const DirectX::SimpleMath::Vector3 CABINET_POSITION = { 3.0f, 0.0f, -3.0f };
	// ���A���̈ʒu
	const DirectX::SimpleMath::Vector3 POTTED_POSITION = { -3.0f, 0.0f, 3.0f };
	// �x�b�h�̈ʒu
	const DirectX::SimpleMath::Vector3 BED_POSITION = { -3.0f, 0.0f, -3.0f };
	// �����^���̈ʒu
	const DirectX::SimpleMath::Vector3 LANTERN_POSITION = { -3.0f,0.0f,0.0f };
	// ���̈ʒu
	const DirectX::SimpleMath::Vector3 DESK_POSITION = { 0.0f,0.0f,2.0f };

	// ------------------------------------- //
	// �摜�̈ʒu�̒萔��
	// ------------------------------------- //
	// �u���{�b�g�̈ړ��v�摜�̈ʒu
	const DirectX::SimpleMath::Vector2 MOVE_TEXTURE_POSITION = { 1135.0f, 450.0f };
	// �u���{�b�g�̏㔼�g�̉�]�v�摜�̈ʒu
	const DirectX::SimpleMath::Vector2 BODY_TEXTURE_POSITION = { 1145.0f, 550.0f };
	// �u���r(���C�g)�̉�]�v�摜�̈ʒu
	const DirectX::SimpleMath::Vector2 RIGHT_TEXTURE_POSITION = { 1120.0f, 640.0f };
	// �uE���ׂ�v�uE����v�摜�̈ʒu
	const DirectX::SimpleMath::Vector2 E_TEXTURE_POSITION = { 200.0f,200.0f };
	// �u���̋ʁv�摜�̈ʒu
	const DirectX::SimpleMath::Vector2 LIGHTBALL_POSITION = { 120.0f,615.0f };
	// �u���v�摜�̈ʒu
	const DirectX::SimpleMath::Vector2 KEY_POSITION = { 105.0f,592.0f };
	// �u�X�e�[�W�N���A�v�摜�̈ʒu
	const DirectX::SimpleMath::Vector2 STAGECLEAR_POSITION = { 50.0f, 100.0f };
	// �u�X�^�[�g�ē��v�摜�̈ʒu
	const DirectX::SimpleMath::Vector2 GUIDE_POSITION = { 100.0f,200.0f };
	// �u�����N���Ȃ��悤���v�摜�̈ʒu
	const DirectX::SimpleMath::Vector2 NOTHING_POSITION = { 100.0f,200.0f };
	// �u�A�C�e���t���[���v�摜�̈ʒu
	const DirectX::SimpleMath::Vector2 ITEMFRAME_POSITION = { 5.0f,605.0f };
	// �u�o�[���v�摜�̈ʒu
	const DirectX::SimpleMath::Vector2 CROWBAR_POSITION = { 10.0f,612.0f };
	// �u�n�}�v�摜�̈ʒu
	const DirectX::SimpleMath::Vector2 MAP_POSITION = { 150.0f,25.0f };

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

	// �V���h�E�}�b�v�p�i�����_�[�e�N�X�`���j
	std::unique_ptr<DX::RenderTexture> m_shadowMapRT;

	// �V���h�E�}�b�v�p�i�f�v�X�X�e���V���j
	std::unique_ptr<Imase::DepthStencil> m_shadowMapDS;

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

	// OBB����֐�
	bool HitCheckOBB(
		const DirectX::BoundingOrientedBox& box1,
		const DirectX::BoundingOrientedBox& box2
	);

	// �Փ˔���p�̕\���I�u�W�F�N�g�ւ̃|�C���^
	//std::unique_ptr<Imase::DisplayCollision> m_displayCollision;

	// �Փ˔���p�I�u�W�F�N�g
	CollisionTest::Object m_robotColl[5];	// ���{�b�g
	CollisionTest::Object m_modelColl[8];	// �e���f��(�󔠓�)
	CollisionTest::Object m_wallColl[4];	// ��
};

