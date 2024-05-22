#include "pch.h"
#include "GameStageScene.h"
#include "DebugDraw.h"
#include "ReadData.h"
#include "StageSelectScene.h"

using namespace DirectX;

#define WALL_NUM 4		// �ǂ̐�
#define OBJECT_NUM 7	// ���f���̐�

GameStageScene::GameStageScene()
	: m_lightRotate{}	  , m_lightPosition{}
	, m_lightTheta(45.0f) , m_distance(0.0f)
	, m_fireFlag(false)   , m_lightBallFlag(false)
	, m_nothingFlag(false), m_lightgetFlag(false)
	, m_keygetFlag(false) , m_keyFlag(false)
	, m_lockFlag(false)	  , m_guideFlag(true)
	, m_notgetFlag(false) , m_stageClearFlag(false)
	, m_crowbarFlag(false), m_bargetFlag(false)
	, m_stuckFlag(false)  , m_barOpenFlag(false)
	, m_keyOpenFlag(false), m_mapFlag(false)
{
}

void GameStageScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();

	// �f�o�b�O�J�����̍쐬
	RECT rect = GetUserResources()->GetDeviceResources()->GetOutputSize();
	m_debugCamera = std::make_unique<Imase::DebugCamera>(rect.right, rect.bottom);

	// �J�����Ƀv���C���[�Ɉʒu�Ɖ�]��n��
	m_camera.SetPlayer(m_robotPosition, m_robotRotate);

	// ���{�b�g�̊e�p�[�c�̍쐬
	m_parts[ROOT] = std::make_unique<Imase::ModelPart>();
	m_parts[HEAD] = std::make_unique<Imase::ModelPart>(m_headModel.get());
	m_parts[BODY] = std::make_unique<Imase::ModelPart>(m_bodyModel.get());
	m_parts[LEG]  = std::make_unique<Imase::ModelPart>(m_legModel.get());
	m_parts[ARM_R] = std::make_unique<Imase::ModelPart>(m_armRModel.get());
	m_parts[ARM_L] = std::make_unique<Imase::ModelPart>(m_armLModel.get());
	m_parts[LIGHT] = std::make_unique<Imase::ModelPart>(m_lightModel.get());
	m_parts[MISSILE] = std::make_unique<Imase::ModelPart>(m_missileModel.get());

	// ���{�b�g�̊e�p�[�c��A������
	m_parts[ROOT]->SetChild(m_parts[LEG].get());
	m_parts[LEG]->SetChild(m_parts[BODY].get());
	m_parts[BODY]->SetChild(m_parts[HEAD].get());
	m_parts[HEAD]->SetSibling(m_parts[ARM_R].get());
	m_parts[ARM_R]->SetSibling(m_parts[ARM_L].get());
	m_parts[ARM_R]->SetChild(m_parts[MISSILE].get());
	m_parts[ARM_L]->SetChild(m_parts[LIGHT].get());
	// �e�p�[�c�̏����ʒu�s����쐬����
	m_parts[BODY]->SetInitialMatrix(
		SimpleMath::Matrix::CreateTranslation(0.0f, 0.32f, 0.0f));
	m_parts[HEAD]->SetInitialMatrix(
		SimpleMath::Matrix::CreateTranslation(0.0f, 0.75f, 0.0f));
	m_parts[ARM_R]->SetInitialMatrix(
		SimpleMath::Matrix::CreateTranslation(-0.18f, 0.72f, 0.0f));
	// ���r�Ɉʒu���
	m_leftArmPosition = SimpleMath::Vector3(0.18f, 0.72f, 0.0f);
	m_parts[ARM_L]->SetInitialMatrix(
		SimpleMath::Matrix::CreateTranslation(m_leftArmPosition));
	m_parts[LIGHT]->SetInitialMatrix(
		SimpleMath::Matrix::CreateTranslation(0.28f, 0.52f, 0.39f));
	m_parts[MISSILE]->SetInitialMatrix(
		SimpleMath::Matrix::CreateTranslation(-0.28f, 0.52f, 0.39f));

	// �A������Ă���p�[�c�̏����ʒu�s���e�̍��W�n�ɑS�ĕϊ�����
	m_parts[ROOT]->SetupMatrix();

	// �萔�o�b�t�@�̓��e�X�V
	ConstantBuffer2 cb = {};
	cb.fCosTheta = cosf(XMConvertToRadians(m_lightTheta / 2.0f));
	context->UpdateSubresource(m_constantBuffer2.Get(), 0, nullptr, &cb, 0, 0);

	// ���Ă���V�[�����I�[�v��������
	auto transitionMask = GetUserResources()->GetTransitionMask();
	transitionMask->Open();

	// ---------------------------------------------- //
	// ���{�b�g���f���̃R���W�����ݒ�
	// ---------------------------------------------- //
	// ���{�b�g�i�́j
	m_robotColl[0].boundingBox = m_bodyModel->meshes[0]->boundingBox;
	// ���{�b�g�i�r�j
	m_robotColl[1].boundingBox = m_legModel->meshes[0]->boundingBox;
	// ���{�b�g�i�E�r�j
	m_robotColl[2].boundingBox = m_armRModel->meshes[0]->boundingBox;
	// ���{�b�g�i���r�j
	m_robotColl[3].boundingBox = m_armLModel->meshes[0]->boundingBox;
	// ���{�b�g�i�~�T�C���j
	m_robotColl[4].boundingBox = m_missileModel->meshes[0]->boundingBox;

	// ---------------------------------------------- //
	// �e���f���̃R���W�����ݒ�
	// ---------------------------------------------- //
	// ��
	m_modelColl[0].boundingBox = m_treasureModel->meshes[0]->boundingBox;
	// WEC�ؔ�
	m_modelColl[1].boundingBox = m_treasureModel->meshes[0]->boundingBox;
	// �ؔ�
	m_modelColl[2].boundingBox = m_woodBoxModel->meshes[0]->boundingBox;
	// �˒I
	m_modelColl[3].boundingBox = m_cabinetModel->meshes[0]->boundingBox;
	// ���A��
	m_modelColl[4].boundingBox = m_pottedModel->meshes[0]->boundingBox;
	// �x�b�h
	m_modelColl[5].boundingBox = m_bedModel->meshes[0]->boundingBox;
	// ��
	m_modelColl[6].boundingBox = m_deskModel->meshes[0]->boundingBox;
	// �����^��
	m_modelColl[7].boundingBox = m_lanternModel->meshes[0]->boundingBox;

	// �󔠂̏Փ˔���̍��W�ݒ�
	m_modelColl[0].position = SimpleMath::Vector3(TREASURE_POSITION);
	// WEC�ؔ��̏Փ˔���̍��W�ݒ�
	m_modelColl[1].position = SimpleMath::Vector3(WECBOX_POSITION);
	// �ؔ��̏Փ˔���̍��W�ݒ�
	m_modelColl[2].position = SimpleMath::Vector3(WOODBOX_POSITION);
	// �˒I�̏Փ˔���̍��W�ݒ�
	m_modelColl[3].position = SimpleMath::Vector3(CABINET_POSITION);
	// ���A���̏Փ˔���̍��W�ݒ�
	m_modelColl[4].position = SimpleMath::Vector3(POTTED_POSITION);
	// �x�b�h�̏Փ˔���̍��W�ݒ�
	m_modelColl[5].position = SimpleMath::Vector3(BED_POSITION);
	// ���̏Փ˔���̍��W�ݒ�
	m_modelColl[6].position = SimpleMath::Vector3(DESK_POSITION);
	// �����^���̏Փ˔���̍��W�ݒ�
	m_modelColl[7].position = SimpleMath::Vector3(LANTERN_POSITION);

	// ---------------------------------------------- //
	// �ǂ̃R���W�����ݒ�
	// ---------------------------------------------- //
	m_wallColl[0].boundingBox = m_wallModel->meshes[0]->boundingBox;	// ���ǂ̑傫��
	m_wallColl[1].boundingBox = m_wallModel->meshes[0]->boundingBox;	// �E�ǂ̑傫��
	m_wallColl[2].boundingBox = m_wallModel->meshes[0]->boundingBox;	// �^���ʂ̕ǂ̑傫��
	m_wallColl[3].boundingBox = m_wallModel->meshes[0]->boundingBox;	// �^���̕ǂ̑傫��

	m_wallColl[0].position = SimpleMath::Vector3(5.0f, 0.0f, 0.0f);		// ���ǂ̈ʒu
	m_wallColl[1].position = SimpleMath::Vector3(-5.0f, 0.0f, 0.0f);	// �E�ǂ̈ʒu
	m_wallColl[2].position = SimpleMath::Vector3(0.0f, 0.0f, 5.0f);		// �^���ʂ̕ǂ̈ʒu
	m_wallColl[3].position = SimpleMath::Vector3(0.0f, 0.0f, -5.0f);	// �^���̕ǂ̈ʒu

	m_wallColl[0].rotate = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, XMConvertToRadians(90.0f));
	m_wallColl[1].rotate = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, XMConvertToRadians(90.0f));
}

void GameStageScene::Update(float elapsedTime)
{
	elapsedTime;

	auto transitionMask = GetUserResources()->GetTransitionMask();
	auto kbTracker = GetUserResources()->GetKeyboardStateTracker();
	auto kb = kbTracker->GetLastState();

	// �f�o�b�O�J�����̍X�V
	m_debugCamera->Update();

	//=====���{�b�g�̈ړ�=====//
	// �e�ē��̉摜���o�Ă���ꍇ�͏������Ȃ�
	if (!m_bargetFlag  && !m_barOpenFlag && !m_lockFlag     &&
		!m_guideFlag   && !m_notgetFlag  && !m_nothingFlag  &&
		!m_bargetFlag  && !m_stuckFlag   && !m_barOpenFlag  &&
		!m_keyOpenFlag && !m_mapFlag	 && !m_lightgetFlag && !m_keygetFlag)
	{
		// �O�i
		if (kb.W)
		{
			m_robotPosition += SimpleMath::Vector3::Transform(SimpleMath::Vector3(0.0f, 0.0f, 0.05f), m_robotRotate);
		}
		// ��i
		if (kb.S)
		{
			m_robotPosition += SimpleMath::Vector3::Transform(SimpleMath::Vector3(0.0f, 0.0f, -0.05f), m_robotRotate);
		}
		// ������
		if (kb.A)
		{
			m_robotRotate *= SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, XMConvertToRadians(1.5f));
		}
		// �E����
		if (kb.D)
		{
			m_robotRotate *= SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, XMConvertToRadians(-1.5f));
		}
	}

	//=====���{�b�g�̏㔼�g�̉�]=====//
	if (kb.Left)
	{
		m_bodyRotate *= SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, XMConvertToRadians(1.5f));
	}
	if (kb.Right)
	{
		m_bodyRotate *= SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, XMConvertToRadians(-1.5f));
	}

	//=====���{�b�g�̍��r�̉�]=====//
	if (kb.Up)
	{
		m_armLRotate *= SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitX, XMConvertToRadians(-1.5f));
	}
	if (kb.Down)
	{
		m_armLRotate *= SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitX, XMConvertToRadians(1.5f));
	}

	//���{�b�g��] *  �̉�] * �r�̉�]
	m_lightRotate = m_robotRotate * m_bodyRotate * m_armLRotate;

	//���C�g�����{�b�g�̘r�Ɉړ�������
	m_lightPosition = m_robotPosition + m_leftArmPosition;

	// �X�y�[�X�L�[�Ń~�T�C���𔭎�
	if (!m_fireFlag && kbTracker->pressed.Space)
	{
		m_fireFlag = true;
		// �~�T�C�������r����O��
		m_parts[ARM_R]->SetChild(nullptr);
		// �~�T�C���̃��[���h�s����擾
		SimpleMath::Matrix m = m_parts[MISSILE]->GetWorldMatrix();
		// �~�T�C���̈ʒu���擾
		m_missilePosition.x = m._41;
		m_missilePosition.y = m._42;
		m_missilePosition.z = m._43;
		// �~�T�C���̉�]���擾
		m_missileRotate = SimpleMath::Quaternion::CreateFromRotationMatrix(m);
	}

	// �~�T�C�����˒��Ȃ�
	if (m_fireFlag)
	{
		// �~�T�C������]������
		m_missileRotate = SimpleMath::Quaternion::CreateFromAxisAngle(
			SimpleMath::Vector3::UnitZ, XMConvertToRadians(5.0f)) * m_missileRotate;
		// �~�T�C���������Ă�������ֈړ�������
		SimpleMath::Vector3 v(0.0f, 0.0f, 0.15f);
		m_missilePosition += SimpleMath::Vector3::Transform(v, m_missileRotate);
		m_distance += v.Length();
		// �ړ�������5���𒴂�����r�Ƀ~�T�C����߂�
		if (m_distance > 5.0f)
		{
			m_fireFlag = false;
			m_distance = 0.0f;
			m_parts[ARM_R]->SetChild(m_parts[MISSILE].get());
		}
	}

	// ---------------------------------- //
	// �V�[���̐؂�ւ�
	// ---------------------------------- //
	// �X�e�[�W�N���A��Ƀ^�C�g���V�[���֖߂�
	if (m_stageClearFlag  && kbTracker->pressed.Enter)
	{
		// �N���[�Y����
		transitionMask->Close();
	}
	// �N���[�Y������ʂ̃V�[����
	if (m_stageClearFlag  && transitionMask->IsClose() && transitionMask->IsEnd())
	{
		ChangeScene<StageSelectScene>();
	}
	
	// ���{�b�g�̏Փ˔���̍��W�ݒ�
	m_robotColl[0].position = m_robotPosition + SimpleMath::Vector3(0.0f,0.32f,0.0f);		// ���{�b�g�i�́j
	m_robotColl[1].position = m_robotPosition;												// ���{�b�g�i�r�j
	m_robotColl[2].position = m_robotPosition + SimpleMath::Vector3(-0.18f,0.72f,0.0f);		// ���{�b�g�i�E�r�j
	m_robotColl[3].position = m_robotPosition + SimpleMath::Vector3(0.18f, 0.72f, 0.0f);	// ���{�b�g�i���r�j
	m_robotColl[4].position = m_missilePosition;											// ���{�b�g�i�~�T�C���j

	m_robotColl[0].rotate = m_robotRotate;
	m_robotColl[1].rotate = m_robotRotate;
	m_robotColl[2].rotate = m_robotRotate * m_bodyRotate;
	m_robotColl[3].rotate = m_robotRotate * m_bodyRotate;
	m_robotColl[4].rotate = m_robotRotate;

	// -------------------------------------------------- //
	// �ǂ̓����蔻��̉����߂�����
	// -------------------------------------------------- //
	for (int i = 0; i < WALL_NUM; i++)
	{
		if (HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_wallColl[i].GetBoundingOrientedBox()))
		{
			m_robotPosition -= SimpleMath::Vector3::Transform(SimpleMath::Vector3(0.0f, 0.0f, 0.05f), m_robotRotate);
		}
	}

	// �J�����̍X�V
	m_camera.Update(elapsedTime);
}

void GameStageScene::Render()
{
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = GetUserResources()->GetCommonStates();

	// -------------------------------------------------------------------------- //
	// �V���h�E�}�b�v�̍쐬
	// -------------------------------------------------------------------------- //

	auto rtv = m_shadowMapRT->GetRenderTargetView();
	auto srv = m_shadowMapRT->GetShaderResourceView();
	auto dsv = m_shadowMapDS->GetDepthStencilView();

	// �����_�[�^�[�Q�b�g��ύX�ishadowMapRT�j
	context->ClearRenderTargetView(rtv, SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f));
	context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
	context->OMSetRenderTargets(1, &rtv, dsv);

	// �r���[�|�[�g��ݒ�
	D3D11_VIEWPORT vp = { 0.0f, 0.0f, SHADOWMAP_SIZE, SHADOWMAP_SIZE, 0.0f, 1.0f };
	context->RSSetViewports(1, &vp);

	m_view = SimpleMath::Matrix::CreateLookAt(
		m_camera.GetEyePosition(),
		m_camera.GetTargetPosition(),
		SimpleMath::Vector3::UnitY
	);

	// ------------------------------------------------ //
	// ���C�g��Ԃ̃r���[�s��Ǝˉe�s����쐬����
	// ------------------------------------------------ //

	// ���C�g�̕���
	SimpleMath::Vector3 lightDir = SimpleMath::Vector3::Transform(SimpleMath::Vector3(0.0f, 0.0f, 1.0f), m_lightRotate);

	// �r���[�s����쐬
	SimpleMath::Matrix view = SimpleMath::Matrix::CreateLookAt(
		m_lightPosition,
		m_lightPosition + lightDir,
		SimpleMath::Vector3::UnitY
	);

	// �ˉe�s����쐬
	SimpleMath::Matrix proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(m_lightTheta), 1.0f, 0.1f, 100.0f);

	// -------------------------------------------------------------------------- //
	// �萔�o�b�t�@���X�V
	// -------------------------------------------------------------------------- //

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	// GPU���萔�o�b�t�@�ɑ΂��ăA�N�Z�X���s��Ȃ��悤�ɂ���
	context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// �萔�o�b�t�@���X�V
	ConstantBuffer cb = {};
	SimpleMath::Matrix m = view * proj;
	cb.lightViewProj = XMMatrixTranspose(m);
	cb.lightPosition = m_lightPosition;
	cb.lightDirection = lightDir;
	cb.lightAmbient = SimpleMath::Color(0.15f, 0.15f, 0.15f);	// �X�e�[�W�̖��邳����

	*static_cast<ConstantBuffer*>(mappedResource.pData) = cb;

	// GPU���萔�o�b�t�@�ɑ΂��ẴA�N�Z�X��������
	context->Unmap(m_constantBuffer.Get(), 0);

	// ------------------------------------------------ //
	// �e�ɂȂ郂�f����`�悷��
	// ------------------------------------------------ //

	SimpleMath::Matrix shadow;

	// �󔠂̉e�̕`��
	shadow = SimpleMath::Matrix::CreateTranslation(TREASURE_POSITION);	// �󔠂̉e�̈ʒu
	m_treasureModel->Draw(context, *states, shadow, view, proj, false, [&]()
		{
			context->VSSetShader(m_VS_Depth.Get(), nullptr, 0);
			context->PSSetShader(m_PS_Depth.Get(), nullptr, 0);
		}
	);

	// WEC�ؔ��̉e�̕`��
	shadow = SimpleMath::Matrix::CreateTranslation(WECBOX_POSITION);	// WEC�ؔ��̉e�̈ʒu
	m_WECBoxModel->Draw(context, *states, shadow, view, proj, false, [&]()
		{
			context->VSSetShader(m_VS_Depth.Get(), nullptr, 0);
			context->PSSetShader(m_PS_Depth.Get(), nullptr, 0);
		}
	);

	// �ؔ��̉e�̕`��
	shadow = SimpleMath::Matrix::CreateTranslation(WOODBOX_POSITION);	// �ؔ��̉e�̈ʒu
	m_woodBoxModel->Draw(context, *states, shadow, view, proj, false, [&]()
		{
			context->VSSetShader(m_VS_Depth.Get(), nullptr, 0);
			context->PSSetShader(m_PS_Depth.Get(), nullptr, 0);
		}
	);

	// �˒I�̉e�̕`��
	shadow = SimpleMath::Matrix::CreateTranslation(CABINET_POSITION);	// �˒I�̉e�̈ʒu
	m_cabinetModel->Draw(context, *states, shadow, view, proj, false, [&]()
		{
			context->VSSetShader(m_VS_Depth.Get(), nullptr, 0);
			context->PSSetShader(m_PS_Depth.Get(), nullptr, 0);
		}
	);

	// ���A���̉e�̕`��
	shadow = SimpleMath::Matrix::CreateTranslation(POTTED_POSITION);	// ���A���̉e�̈ʒu
	m_pottedModel->Draw(context, *states, shadow, view, proj, false, [&]()
		{
			context->VSSetShader(m_VS_Depth.Get(), nullptr, 0);
			context->PSSetShader(m_PS_Depth.Get(), nullptr, 0);
		}
	);

	// �x�b�h�̉e�̕`��
	shadow = SimpleMath::Matrix::CreateTranslation(BED_POSITION);	// �x�b�h�̉e�̈ʒu
	m_bedModel->Draw(context, *states, shadow, view, proj, false, [&]()
		{
			context->VSSetShader(m_VS_Depth.Get(), nullptr, 0);
			context->PSSetShader(m_PS_Depth.Get(), nullptr, 0);
		}
	);

	// �����^���̉e�̕`��
	shadow = SimpleMath::Matrix::CreateTranslation(LANTERN_POSITION);	// �����^���̉e�̈ʒu
	m_lanternModel->Draw(context, *states, shadow, view, proj, false, [&]()
		{
			context->VSSetShader(m_VS_Depth.Get(), nullptr, 0);
			context->PSSetShader(m_PS_Depth.Get(), nullptr, 0);
		}
	);

	// ���̉e�̕`��
	shadow = SimpleMath::Matrix::CreateTranslation(DESK_POSITION);	// ���̉e�̈ʒu
	m_deskModel->Draw(context, *states, shadow, view, proj, false, [&]()
		{
			context->VSSetShader(m_VS_Depth.Get(), nullptr, 0);
			context->PSSetShader(m_PS_Depth.Get(), nullptr, 0);
		}
	);

	// -------------------------------------------------------------------------- //
	// �����_�[�^�[�Q�b�g�ƃr���[�|�[�g�����ɖ߂�
	// -------------------------------------------------------------------------- //
	auto renderTarget = GetUserResources()->GetDeviceResources()->GetRenderTargetView();
	auto depthStencil = GetUserResources()->GetDeviceResources()->GetDepthStencilView();
	
	auto kbTracker = GetUserResources()->GetKeyboardStateTracker();
	auto kb = kbTracker->GetLastState();

	context->ClearRenderTargetView(renderTarget, Colors::Black);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);
	auto const viewport = GetUserResources()->GetDeviceResources()->GetScreenViewport();
	context->RSSetViewports(1, &viewport);


	// ---------------------------------------------------------------- //
	// �e���f���̕`��
	// ---------------------------------------------------------------- //
	SimpleMath::Matrix model;
															// �����̑傫���ύX
	model = SimpleMath::Matrix::CreateScale(SimpleMath::Vector3(1.0f,1.0f,1.0f)) * SimpleMath::Matrix::CreateTranslation(0.0f, 0.0f, 0.0f);

	// ���̕`��
	m_floorModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
		{
			// �萔�o�b�t�@�̐ݒ�
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// �쐬�����V���h�E�}�b�v�����\�[�X�Ƃ��Đݒ�
			context->PSSetShaderResources(1, 1, &srv);

			// �e�N�X�`���T���v���[�̐ݒ�
			ID3D11SamplerState* samplers[] = { states->PointWrap(), m_shadowMapSampler.Get()};
			context->PSSetSamplers(0, 2, samplers);

			// �V�F�[�_�[�̐ݒ�
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	// --------------------------------------------------------------------------------- //
	// �ǃ��f���̕`��
	// --------------------------------------------------------------------------------- // 
												// ���ǂ̈ʒu�ύX
	model = SimpleMath::Matrix::CreateTranslation(0.0f, 0.0f, 5.0f);
	// �ǂ̕`��i�^���ʁj
	m_wallModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
		{
			// �萔�o�b�t�@�̐ݒ�
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// �쐬�����V���h�E�}�b�v�����\�[�X�Ƃ��Đݒ�
			context->PSSetShaderResources(1, 1, &srv);

			// �e�N�X�`���T���v���[�̐ݒ�
			ID3D11SamplerState* samplers[] = { states->PointWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// �V�F�[�_�[�̐ݒ�
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	model = SimpleMath::Matrix::CreateTranslation(0.0f, 0.0f, 5.0f)
		* SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(90.0f));
	// �ǂ̕`��i���ǁj
	m_wallModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
		{
			// �萔�o�b�t�@�̐ݒ�
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// �쐬�����V���h�E�}�b�v�����\�[�X�Ƃ��Đݒ�
			context->PSSetShaderResources(1, 1, &srv);

			// �e�N�X�`���T���v���[�̐ݒ�
			ID3D11SamplerState* samplers[] = { states->PointWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// �V�F�[�_�[�̐ݒ�
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	model = SimpleMath::Matrix::CreateTranslation(0.0f, 0.0f, -5.0f)
		* SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(90.0f));
	// �ǂ̕`��i�E�ǁj
	m_wallModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
		{
			// �萔�o�b�t�@�̐ݒ�
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// �쐬�����V���h�E�}�b�v�����\�[�X�Ƃ��Đݒ�
			context->PSSetShaderResources(1, 1, &srv);

			// �e�N�X�`���T���v���[�̐ݒ�
			ID3D11SamplerState* samplers[] = { states->PointWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// �V�F�[�_�[�̐ݒ�
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	model = SimpleMath::Matrix::CreateTranslation(0.0f, 0.0f, -5.0f);
	// �ǂ̕`��i�^���j
	m_wallModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
		{
			// �萔�o�b�t�@�̐ݒ�
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// �쐬�����V���h�E�}�b�v�����\�[�X�Ƃ��Đݒ�
			context->PSSetShaderResources(1, 1, &srv);

			// �e�N�X�`���T���v���[�̐ݒ�
			ID3D11SamplerState* samplers[] = { states->PointWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// �V�F�[�_�[�̐ݒ�
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	// ---------------------------------------------------------------------------- //
	// �e���f���̕`��
	// ---------------------------------------------------------------------------- //
	// �󔠂̕`��
	model = SimpleMath::Matrix::CreateTranslation(TREASURE_POSITION);	// �󔠂̈ʒu
	m_treasureModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
		{
			// �萔�o�b�t�@�̐ݒ�
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// �쐬�����V���h�E�}�b�v�����\�[�X�Ƃ��Đݒ�
			context->PSSetShaderResources(1, 1, &srv);

			// �e�N�X�`���T���v���[�̐ݒ�
			ID3D11SamplerState* samplers[] = { states->LinearWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// �V�F�[�_�[�̐ݒ�
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	// WEC�ؔ��̕`��
	model = SimpleMath::Matrix::CreateTranslation(WECBOX_POSITION);	// WEC�ؔ��̈ʒu
	m_WECBoxModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
		{
			// �萔�o�b�t�@�̐ݒ�
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// �쐬�����V���h�E�}�b�v�����\�[�X�Ƃ��Đݒ�
			context->PSSetShaderResources(1, 1, &srv);

			// �e�N�X�`���T���v���[�̐ݒ�
			ID3D11SamplerState* samplers[] = { states->LinearWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// �V�F�[�_�[�̐ݒ�
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	// �ؔ��̕`��
	model = SimpleMath::Matrix::CreateTranslation(WOODBOX_POSITION); // �ؔ��̈ʒu
	m_woodBoxModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
		{
			// �萔�o�b�t�@�̐ݒ�
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// �쐬�����V���h�E�}�b�v�����\�[�X�Ƃ��Đݒ�
			context->PSSetShaderResources(1, 1, &srv);

			// �e�N�X�`���T���v���[�̐ݒ�
			ID3D11SamplerState* samplers[] = { states->LinearWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// �V�F�[�_�[�̐ݒ�
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	// �˒I�̕`��
	model = SimpleMath::Matrix::CreateTranslation(CABINET_POSITION); // �˒I�̈ʒu
	m_cabinetModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
		{
			// �萔�o�b�t�@�̐ݒ�
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// �쐬�����V���h�E�}�b�v�����\�[�X�Ƃ��Đݒ�
			context->PSSetShaderResources(1, 1, &srv);

			// �e�N�X�`���T���v���[�̐ݒ�
			ID3D11SamplerState* samplers[] = { states->LinearWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// �V�F�[�_�[�̐ݒ�
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	// ���A���̕`��
	model = SimpleMath::Matrix::CreateTranslation(POTTED_POSITION); // ���A���̈ʒu
	m_pottedModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
		{
			// �萔�o�b�t�@�̐ݒ�
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// �쐬�����V���h�E�}�b�v�����\�[�X�Ƃ��Đݒ�
			context->PSSetShaderResources(1, 1, &srv);

			// �e�N�X�`���T���v���[�̐ݒ�
			ID3D11SamplerState* samplers[] = { states->LinearWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// �V�F�[�_�[�̐ݒ�
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	// �x�b�h�̕`��
	model = SimpleMath::Matrix::CreateTranslation(BED_POSITION); // �x�b�h�̈ʒu
	m_bedModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
		{
			// �萔�o�b�t�@�̐ݒ�
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// �쐬�����V���h�E�}�b�v�����\�[�X�Ƃ��Đݒ�
			context->PSSetShaderResources(1, 1, &srv);

			// �e�N�X�`���T���v���[�̐ݒ�
			ID3D11SamplerState* samplers[] = { states->LinearWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// �V�F�[�_�[�̐ݒ�
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	// �����^���̕`��
	model = SimpleMath::Matrix::CreateTranslation(LANTERN_POSITION);	// �����^���̈ʒu
	m_lanternModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
		{
			// �萔�o�b�t�@�̐ݒ�
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// �쐬�����V���h�E�}�b�v�����\�[�X�Ƃ��Đݒ�
			context->PSSetShaderResources(1, 1, &srv);

			// �e�N�X�`���T���v���[�̐ݒ�
			ID3D11SamplerState* samplers[] = { states->LinearWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// �V�F�[�_�[�̐ݒ�
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	// ���̕`��
	model = SimpleMath::Matrix::CreateTranslation(DESK_POSITION);	// �����^���̈ʒu
	m_deskModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
		{
			// �萔�o�b�t�@�̐ݒ�
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// �쐬�����V���h�E�}�b�v�����\�[�X�Ƃ��Đݒ�
			context->PSSetShaderResources(1, 1, &srv);

			// �e�N�X�`���T���v���[�̐ݒ�
			ID3D11SamplerState* samplers[] = { states->LinearWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// �V�F�[�_�[�̐ݒ�
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	// ���\�[�X�̊��蓖�Ă���������ishadowMapRT�j
	ID3D11ShaderResourceView* nullsrv[] = { nullptr };
	context->PSSetShaderResources(1, 1, nullsrv);

	SimpleMath::Matrix world;
	world = SimpleMath::Matrix::CreateFromQuaternion(m_lightRotate) * SimpleMath::Matrix::CreateTranslation(m_lightPosition);
	
	// ���{�b�g�̏㔼�g�̉�]
	m_parts[BODY]->SetTransformMatrix(SimpleMath::Matrix::CreateFromQuaternion(m_bodyRotate));

	// ���{�b�g�̍��r�̉�]
	m_parts[ARM_L]->SetTransformMatrix(SimpleMath::Matrix::CreateFromQuaternion(m_armLRotate));

	// ���{�b�g���ړ�������
	SimpleMath::Matrix move = SimpleMath::Matrix::CreateFromQuaternion(m_robotRotate)
		* SimpleMath::Matrix::CreateTranslation(m_robotPosition);
	m_parts[ROOT]->SetTransformMatrix(move);

	// �e�̕`��֐�
	DrawShadow(context, states, m_robotPosition, 1.0f);

	// ���{�b�g�̕`��
	m_parts[ROOT]->UpdateMatrix();
	m_parts[ROOT]->Draw(context, *states, m_view, m_proj, [&]()
		{
			// �J�����O���Ȃ�
			context->RSSetState(states->CullNone());
		}
	);

	// �~�T�C�����˒��Ȃ�
	if (m_fireFlag)
	{
		// �~�T�C����`�悷��
		SimpleMath::Matrix missile = SimpleMath::Matrix::CreateFromQuaternion(m_missileRotate)
			* SimpleMath::Matrix::CreateTranslation(m_missilePosition);
		m_parts[MISSILE]->GetModel()->Draw(context, *states, missile, m_view, m_proj);
	}


	// ----------------------------------------- //
	// �摜�̕`�悱������
	// ----------------------------------------- //
	m_spriteBatch->Begin();

	// �u���{�b�g�̈ړ��v�摜�̕`��
	m_spriteBatch->Draw(m_robotMoveSRV.Get(), SimpleMath::Vector2(MOVE_TEXTURE_POSITION));

	// �u���{�b�g�̏㔼�g�̉�]�v�摜�̕`��
	m_spriteBatch->Draw(m_upbodySRV.Get(), SimpleMath::Vector2(BODY_TEXTURE_POSITION));

	// �u���r(���C�g)�̉�]�v�摜�̕`��
	m_spriteBatch->Draw(m_ArmLSRV.Get(), SimpleMath::Vector2(RIGHT_TEXTURE_POSITION));

	// �u�X�^�[�g�t���[���v�摜�̕`��
	m_spriteBatch->Draw(m_itemFrameSRV.Get(), SimpleMath::Vector2(ITEMFRAME_POSITION));

	// �u�X�^�[�g�ē��v�摜�̕`��
	if (m_guideFlag )
	{
		m_spriteBatch->Draw(m_guideSRV.Get(), SimpleMath::Vector2(GUIDE_POSITION));
	}
	if (m_guideFlag  && kb.Enter)
	{
		// �摜�̍폜
		m_guideFlag = false;
	}

	// ���{�b�g�ƃ����^��������������
	if (HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[7].GetBoundingOrientedBox()))
	{
		// �uE�͂߂�v�摜�̕`��
		m_spriteBatch->Draw(m_eSetSRV.Get(), SimpleMath::Vector2(E_TEXTURE_POSITION));
	}

	// ���{�b�g�Ɗe���f��������������
	for (int i = 0; i < OBJECT_NUM; i++)
	{
		if (HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[i].GetBoundingOrientedBox()))
		{
			// �uE���ׂ�v�摜�̕`��
			m_spriteBatch->Draw(m_eCheckSRV.Get(), SimpleMath::Vector2(E_TEXTURE_POSITION));
		}
	}

	// ---------------------------------- //
	// �u�����Ȃ��悤���v�摜�̕`��
	// ---------------------------------- //
	// �����蔻��
	if (HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[2].GetBoundingOrientedBox()) && kb.E ||
		HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[4].GetBoundingOrientedBox()) && kb.E ||
		HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[5].GetBoundingOrientedBox()) && kb.E)
	{
		m_nothingFlag = true;
	}
	// �u�����Ȃ��悤���v�摜�`��
	if (m_nothingFlag )
	{
		m_spriteBatch->Draw(m_nothingSRV.Get(), SimpleMath::Vector2(GUIDE_POSITION));
	}
	// �u�����Ȃ��悤���v�摜�̍폜
	if (m_nothingFlag  && kb.Enter)
	{
		m_nothingFlag = false;
	}

	// ---------------------------------- //
	// �u�����N���Ȃ��v�摜�̕`��
	// ---------------------------------- //
	if (HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[7].GetBoundingOrientedBox())
		&& !m_notgetFlag && !m_lightBallFlag && kb.E)
	{
		m_notgetFlag = true;
	}
	if (m_notgetFlag  && !m_stageClearFlag)
	{
		m_spriteBatch->Draw(m_notgetSRV.Get(), SimpleMath::Vector2(NOTHING_POSITION));
	}
	if (m_notgetFlag  && kb.Enter)
	{
		m_notgetFlag = false;	// �摜�̍폜
	}

	// ------------------------------------------ //
	// �u�����|�����Ă��ĊJ���Ȃ��v�摜�̕`��
	// ------------------------------------------ //
	if (!m_crowbarFlag &&
		HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[3].GetBoundingOrientedBox()) && kb.E)
	{
		m_stuckFlag = true;
	}
	if (m_stuckFlag )
	{
		m_spriteBatch->Draw(m_stuckSRV.Get(), SimpleMath::Vector2(GUIDE_POSITION));
	}
	// �u�����|�����Ă��ĊJ���Ȃ��v�摜�̍폜
	if (m_stuckFlag  && kb.Enter)
	{
		m_stuckFlag = false;
	}

	// -------------------------------- //
	// ���Ɍ��̋ʂ��������Ă���Ȃ�
	// -------------------------------- //
	if (m_lightBallFlag  && 
		HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[0].GetBoundingOrientedBox()) && kb.E)
	{
		m_nothingFlag = true;
	}
	// �u�����Ȃ��悤���v�摜�̕`��
	if (m_nothingFlag)
	{
		m_spriteBatch->Draw(m_nothingSRV.Get(), SimpleMath::Vector2(GUIDE_POSITION));
	}

	// ---------------------------------------- //
	// ���Ɍ��������Ă���̂Ȃ�
	// ---------------------------------------- //
	if (m_keyFlag  && HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[3].GetBoundingOrientedBox()) && kb.E
		|| m_lightBallFlag  && HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[3].GetBoundingOrientedBox()) && kb.E)
	{
		m_nothingFlag = true;
	}
	// �u�����Ȃ��悤���v�摜�̕`��
	if (m_nothingFlag)
	{
		m_spriteBatch->Draw(m_nothingSRV.Get(), SimpleMath::Vector2(GUIDE_POSITION));
	}

	// -------------------------------- //
	// ���Ƀo�[�����������Ă���Ȃ�
	// -------------------------------- //
	if (m_crowbarFlag  &&
		HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[1].GetBoundingOrientedBox()) && kb.E)
	{
		m_nothingFlag = true;
	}
	// �u�����Ȃ��悤���v�摜�̕`��
	if (m_nothingFlag)
	{
		m_spriteBatch->Draw(m_nothingSRV.Get(), SimpleMath::Vector2(GUIDE_POSITION));
	}

	// ------------------------------------------- //
	// �˒I�ƐڐG����E�L�[����������o�[�����擾
	// ------------------------------------------- //
	// �����蔻��
	if (!m_crowbarFlag &&	// �o�[���������Ă��Ȃ���
		HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[1].GetBoundingOrientedBox()) && kb.E)
	{
		m_bargetFlag = true;
		m_crowbarFlag = true;
	}
	// �u�o�[���l���v�摜�̕`��
	if (m_bargetFlag)
	{
		m_spriteBatch->Draw(m_bargetSRV.Get(), SimpleMath::Vector2(GUIDE_POSITION));
	}
	// �u�o�[���l���v�摜�̍폜
	if (m_bargetFlag && kb.Enter)
	{
		m_bargetFlag = false;
	}
	// �u�o�[���v�摜�̕`��
	if (m_crowbarFlag)
	{
		m_spriteBatch->Draw(m_crowBarSRV.Get(), SimpleMath::Vector2(CROWBAR_POSITION));
	}

	// -------------------------------------------- //
	// �˒I�ƐڐG����E�L�[����������u���v���擾
	// -------------------------------------------- //
	// �����蔻��
	if (m_crowbarFlag  && !m_keyFlag && !m_lightBallFlag &&	// �o�[���������Ă���A���ƌ��̋ʂ������Ă��Ȃ���
		HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[3].GetBoundingOrientedBox()) && kb.E)
	{
		m_barOpenFlag = true;
	}
	// �u�o�[���������J�����v�摜�̕`��
	if (m_barOpenFlag)
	{
		m_spriteBatch->Draw(m_barOpenSRV.Get(), SimpleMath::Vector2(GUIDE_POSITION));
	}
	// �u���l���v�摜�̕`��
	if (m_keygetFlag)
	{
		m_spriteBatch->Draw(m_keygetSRV.Get(), SimpleMath::Vector2(GUIDE_POSITION));
	}
	// �u���v�摜�̕`��
	if (m_keyFlag)
	{
		m_spriteBatch->Draw(m_keySRV.Get(), SimpleMath::Vector2(KEY_POSITION));
	}

	// ---------------------------------------------------------- //
	// �����{�[���������Ă��Ȃ����ɕ󔠂ɐڐG���AE�L�[����������
	// ---------------------------------------------------------- //
	// �����蔻��
	if (!m_keyFlag && !m_lightBallFlag
		&& HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[0].GetBoundingOrientedBox()) && kb.E)
	{
		m_lockFlag = true;
	}
	// �u�����������Ă���v�摜�̕\��
	if (m_lockFlag)
	{
		m_spriteBatch->Draw(m_lockSRV.Get(), SimpleMath::Vector2(GUIDE_POSITION));
	}
	// �u�����������Ă���v�摜�̍폜
	if (m_lockFlag && kb.Enter)
	{
		m_lockFlag = false;
	}
	// ------------------------------------ //
	// ���������Ă�������̋ʂ��擾����
	// ------------------------------------ //
	if (m_keyFlag && 
		HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[0].GetBoundingOrientedBox()) && kb.E)
	{
		m_keyOpenFlag = true;
		m_keyFlag = false;	// ���摜�̍폜
	}
	// �u���ŊJ�����v�摜�̕`��
	if (m_keyOpenFlag)
	{
		m_spriteBatch->Draw(m_keyOpenSRV.Get(), SimpleMath::Vector2(GUIDE_POSITION));
	}
	// �u���̋ʊl���v�摜�̕`��
	if (m_lightgetFlag)
	{
		m_spriteBatch->Draw(m_lightgetSRV.Get(), SimpleMath::Vector2(GUIDE_POSITION));
	}
	// �u���̋ʁv�摜�̕`��
	if (m_lightBallFlag)
	{
		m_spriteBatch->Draw(m_lightBallSRV.Get(), SimpleMath::Vector2(LIGHTBALL_POSITION));
	}

	// --------------------------------------- //
	// ���ƐڐG����E�L�[����������n�}��\��
	// --------------------------------------- //
	if (HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[6].GetBoundingOrientedBox()) && kb.E)
	{
		m_mapFlag = true;
	}
	// �u�n�}�v�摜�̕`��
	if (m_mapFlag)
	{
		m_spriteBatch->Draw(m_mapSRV.Get(), SimpleMath::Vector2(MAP_POSITION));
	}
	// �u�n�}�v�摜�̍폜
	if (m_mapFlag && kb.Enter)
	{
		m_mapFlag = false;
	}

	// -------------------------------- //
	// �摜�̍폜
	// -------------------------------- //
	// �u�o�[���������J�����v�摜�̍폜
	if (m_barOpenFlag && kbTracker->pressed.Enter)
	{
		m_barOpenFlag = false;
		m_keygetFlag = true;
		m_keyFlag = true;
	}
	// �u���l���v�摜�̍폜
	else if (m_keygetFlag && kbTracker->pressed.Enter)
	{
		m_keygetFlag = false;
	}
	// �u���ŊJ�����v�摜�̍폜
	if (m_keyOpenFlag && kbTracker->pressed.Enter)
	{
		m_keyOpenFlag = false;
		m_lightgetFlag = true;
		m_lightBallFlag = true;
	}
	// �u���̋ʊl���v�摜�̍폜
	else if (m_lightgetFlag && kbTracker->pressed.Enter)
	{
		m_lightgetFlag = false;
	}

	// --------------------------------------- //
	// �u�X�e�[�W�N���A�v�摜�̕`��
	// --------------------------------------- //
	if (HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[7].GetBoundingOrientedBox())
		&& m_lightBallFlag  && kb.E)
	{
		m_stageClearFlag = true;
	}
	if (m_stageClearFlag)
	{
		// �u���̋ʁv�摜�̍폜
		m_lightBallFlag = false;
		m_spriteBatch->Draw(m_stageClearSRV.Get(), SimpleMath::Vector2(STAGECLEAR_POSITION));
	}

	m_spriteBatch->End();

	// �Փ˔���̓o�^�i�f�o�b�O�p�j
	//CollisionRegist();
}

void GameStageScene::Finalize()
{
	m_headModel.reset();
	m_bodyModel.reset();
	m_legModel.reset();
	m_armRModel.reset();
	m_armLModel.reset();
	m_lightModel.reset();
	m_missileModel.reset();

	m_floorModel.reset();
	m_wallModel.reset();

	m_lanternModel.reset();
	m_deskModel.reset();
	m_treasureModel.reset();
	m_WECBoxModel.reset();
	m_woodBoxModel.reset();
	m_cabinetModel.reset();
	m_pottedModel.reset();
	m_bedModel.reset();
	m_lanternModel.reset();
	m_deskModel.reset();

	m_robotMoveSRV.Reset();
	m_upbodySRV.Reset();
	m_ArmLSRV.Reset();
	m_eCheckSRV.Reset();
	m_lightBallSRV.Reset();
	m_nothingSRV.Reset();
	m_lightgetSRV.Reset();
	m_keygetSRV.Reset();
	m_keySRV.Reset();
	m_lockSRV.Reset();
	m_eSetSRV.Reset();
	m_stageClearSRV.Reset();
	m_notgetSRV.Reset();
	m_guideSRV.Reset();
	m_itemFrameSRV.Reset();
	m_crowBarSRV.Reset();
	m_bargetSRV.Reset();
	m_stuckSRV.Reset();
	m_barOpenSRV.Reset();
	m_mapSRV.Reset();

	// �e�̏I������
	ResetShadow();
}

void GameStageScene::CreateDeviceDependentResources()
{
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();

	// �G�t�F�N�g�t�@�N�g���[�̍쐬(���f���p)
	EffectFactory fx(device);
	fx.SetDirectory(L"Resources/Models");

	// ���̃��f���̍쐬
	m_floorModel = Model::CreateFromCMO(device, L"Resources/Models/Floor.cmo", fx);
	// ��
	m_wallModel = Model::CreateFromCMO(device, L"Resources/Models/Wall.cmo", fx);
	// �󔠂̍쐬
	m_treasureModel = Model::CreateFromCMO(device, L"Resources/Models/Treasure.cmo", fx);
	// WEC�ؔ��̍쐬
	m_WECBoxModel = Model::CreateFromCMO(device, L"Resources/Models/WECBox.cmo", fx);
	// �ؔ��̍쐬
	m_woodBoxModel = Model::CreateFromCMO(device, L"Resources/Models/WoodBox.cmo", fx);
	// �˒I�̍쐬
	m_cabinetModel = Model::CreateFromCMO(device, L"Resources/Models/Cabinet.cmo", fx);
	// ���A���̍쐬
	m_pottedModel = Model::CreateFromCMO(device, L"Resources/Models/Potted.cmo", fx);
	// �x�b�h�̍쐬
	m_bedModel = Model::CreateFromCMO(device, L"Resources/Models/Bed.cmo", fx);
	// �����^���̍쐬
	m_lanternModel = Model::CreateFromCMO(device, L"Resources/Models/Lantern.cmo", fx);
	// ���̍쐬
	m_deskModel = Model::CreateFromCMO(device, L"Resources/Models/Desk.cmo", fx);

	// �X�|�b�g���C�g�̃��f���̍쐬
	m_spotLightModel = Model::CreateFromCMO(device, L"Resources/Models/SpotLight.cmo", fx);


	// ---------------------------------------------------------------------- //
	// ���{�b�g�̃��f���̍쐬
	// ---------------------------------------------------------------------- //
	// �G�t�F�N�g�t�@�N�g���[�̍쐬(���{�b�g�p)
	std::unique_ptr<EffectFactory> fxrbt = std::make_unique<EffectFactory>(device);
	fxrbt->SetDirectory(L"Resources/Models");

	// ���{�b�g�̊e�p�[�c���f���̍쐬
	m_headModel = Model::CreateFromCMO(device, L"Resources/Models/Head.cmo", *fxrbt);
	m_bodyModel = Model::CreateFromCMO(device, L"Resources/Models/Body.cmo", *fxrbt);
	m_legModel = Model::CreateFromCMO(device, L"Resources/Models/Leg.cmo", *fxrbt);
	m_armRModel = Model::CreateFromCMO(device, L"Resources/Models/Arm_R.cmo", *fxrbt);
	m_armLModel = Model::CreateFromCMO(device, L"Resources/Models/Arm_L.cmo", *fxrbt);
	m_lightModel = Model::CreateFromCMO(device, L"Resources/Models/SpotLight.cmo", *fxrbt);
	m_missileModel = Model::CreateFromCMO(device, L"Resources/Models/Missile.cmo", *fxrbt);

	// �Փ˔���̕\���I�u�W�F�N�g�̍쐬
	//m_displayCollision = std::make_unique<Imase::DisplayCollision>(device, context);

	// �e�̏������֐�
	InitializeShadow(device, context);

	// ���Ȕ�������悤�ɃG�t�F�N�g��ݒ肷��
	m_spotLightModel->UpdateEffects([&](IEffect* effect)
		{
			auto basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				basicEffect->SetEmissiveColor(Colors::White);
			}
		}
	);

	RECT rect = { 0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE };

	// �����_�[�e�N�X�`���̍쐬�i�V���h�E�}�b�v�p�j
	m_shadowMapRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R32_FLOAT);
	m_shadowMapRT->SetDevice(device);
	m_shadowMapRT->SetWindow(rect);

	// �f�v�X�X�e���V���̍쐬�i�V���h�E�}�b�v�p�j
	m_shadowMapDS = std::make_unique<Imase::DepthStencil>(DXGI_FORMAT_D32_FLOAT);
	m_shadowMapDS->SetDevice(device);
	m_shadowMapDS->SetWindow(rect);

	// �X�v���C�g�o�b�`�̍쐬
	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	// --------------------------------------------------------------------------- //
	// �e�摜�̍쐬
	// --------------------------------------------------------------------------- //
	// �u���{�b�g�̈ړ��v�摜�̍쐬
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/WASD.dds", nullptr, m_robotMoveSRV.ReleaseAndGetAddressOf())
	);
	// �u���{�b�g�̏㔼�g�̉�]�v�摜�̍쐬
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/robot_RL.dds", nullptr, m_upbodySRV.ReleaseAndGetAddressOf())
	);
	// �u���r(���C�g)�̉�]�v�摜�̍쐬
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/ArmL_UD.dds", nullptr, m_ArmLSRV.ReleaseAndGetAddressOf())
	);
	// �uE���ׂ�v�摜�̍쐬
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/E_Check.dds", nullptr, m_eCheckSRV.ReleaseAndGetAddressOf())
	);
	// �u���̋ʁv�摜�̍쐬
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/LightBall.dds", nullptr, m_lightBallSRV.ReleaseAndGetAddressOf())
	);
	// �u�����Ȃ��悤���v�摜�̍쐬
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/NotingGuide.dds", nullptr, m_nothingSRV.ReleaseAndGetAddressOf())
	);
	// �u���̋ʊl���v�摜�̍쐬
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/LightGetGuide.dds", nullptr, m_lightgetSRV.ReleaseAndGetAddressOf())
	);
	// �u���l���v�摜�̍쐬
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/KeyGetGuide.dds", nullptr, m_keygetSRV.ReleaseAndGetAddressOf())
	);
	// �u�����������Ă���v�摜�̍쐬
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/LockedGuide.dds", nullptr, m_lockSRV.ReleaseAndGetAddressOf())
	);
	// �u���v�摜�̍쐬
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/key.dds", nullptr, m_keySRV.ReleaseAndGetAddressOf())
	);
	// �uE�͂߂�v�摜�̍쐬
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/E_Set.dds", nullptr, m_eSetSRV.ReleaseAndGetAddressOf())
	);
	// �u�X�e�[�W�N���A�v�摜�̍쐬
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/STAGECLEAR.dds", nullptr, m_stageClearSRV.ReleaseAndGetAddressOf())
	);
	// �u�����N���Ȃ��v�摜�̍쐬
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/NotingHappen.dds", nullptr, m_notgetSRV.ReleaseAndGetAddressOf())
	);
	// �u�X�^�[�g�ē��v�摜�̍쐬
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/StartGuide.dds", nullptr, m_guideSRV.ReleaseAndGetAddressOf())
	);
	// �u�A�C�e���t���[���v�摜�̍쐬
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/ItemFrame.dds", nullptr, m_itemFrameSRV.ReleaseAndGetAddressOf())
	);
	// �u�o�[���v�摜�̍쐬
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/CrowBar.dds", nullptr, m_crowBarSRV.ReleaseAndGetAddressOf())
	);
	// �u�o�[���v�摜�̍쐬
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/CrawBarGetGuide.dds", nullptr, m_bargetSRV.ReleaseAndGetAddressOf())
	);
	// �u�摜�����|�����Ă��ĊJ���Ȃ��v�摜�̍쐬
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/StuckGuide.dds", nullptr, m_stuckSRV.ReleaseAndGetAddressOf())
	);
	// �u�o�[���������J�����v�摜�̍쐬
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/BarOpenGuide.dds", nullptr, m_barOpenSRV.ReleaseAndGetAddressOf())
	);
	// �u���ŊJ�����v�摜�̍쐬
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/KeyOpenGuide.dds", nullptr, m_keyOpenSRV.ReleaseAndGetAddressOf())
	);
	// �u���ŊJ�����v�摜�̍쐬
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/MapGuide.dds", nullptr, m_mapSRV.ReleaseAndGetAddressOf())
	);

	// ���_�V�F�[�_�[�̍쐬�i�V���h�E�}�b�v�p�j
	std::vector<uint8_t> vs_depth = DX::ReadData(L"Resources/Shaders/SM_VS_Depth.cso");
	DX::ThrowIfFailed(
		device->CreateVertexShader(vs_depth.data(), vs_depth.size(), nullptr, m_VS_Depth.ReleaseAndGetAddressOf())
	);

	// �s�N�Z���V�F�[�_�[�̍쐬�i�V���h�E�}�b�v�p�j
	std::vector<uint8_t> ps_depth = DX::ReadData(L"Resources/Shaders/SM_PS_Depth.cso");
	DX::ThrowIfFailed(
		device->CreatePixelShader(ps_depth.data(), ps_depth.size(), nullptr, m_PS_Depth.ReleaseAndGetAddressOf())
	);

	// �萔�o�b�t�@�̍쐬
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = static_cast<UINT>(sizeof(ConstantBuffer));	// �m�ۂ���T�C�Y�i16�̔{���Őݒ肷��j
	// GPU (�ǂݎ���p) �� CPU (�������ݐ�p) �̗�������A�N�Z�X�ł��郊�\�[�X
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// �萔�o�b�t�@�Ƃ��Ĉ���
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPU�����e��ύX�ł���悤�ɂ���
	DX::ThrowIfFailed(device->CreateBuffer(&bufferDesc, nullptr, m_constantBuffer.ReleaseAndGetAddressOf()));

	// �萔�o�b�t�@�̍쐬
	bufferDesc.ByteWidth = static_cast<UINT>(sizeof(ConstantBuffer2));	// �m�ۂ���T�C�Y�i16�̔{���Őݒ肷��j
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;	// GPU�̓ǂݎ��Ə������݂��\�Ȉ�ʓI�ȃ��\�[�X
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// �萔�o�b�t�@�Ƃ��Ĉ���
	bufferDesc.CPUAccessFlags = 0;	// CPU�̓A�N�Z�X���Ȃ��̂�0
	DX::ThrowIfFailed(device->CreateBuffer(&bufferDesc, nullptr, m_constantBuffer2.ReleaseAndGetAddressOf()));

	// ���_�V�F�[�_�[�̍쐬�i�V���h�E�}�b�v�p�j
	std::vector<uint8_t> vs = DX::ReadData(L"Resources/Shaders/SM_VS.cso");
	DX::ThrowIfFailed(
		device->CreateVertexShader(vs.data(), vs.size(), nullptr, m_VS.ReleaseAndGetAddressOf())
	);

	// �s�N�Z���V�F�[�_�[�̍쐬�i�V���h�E�}�b�v�p�j
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/SM_PS.cso");
	DX::ThrowIfFailed(
		device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_PS.ReleaseAndGetAddressOf())
	);

	// �T���v���[�̍쐬�i�V���h�E�}�b�v�p�j
	D3D11_SAMPLER_DESC sampler_desc = CD3D11_SAMPLER_DESC(D3D11_DEFAULT);
	sampler_desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_LESS;
	device->CreateSamplerState(&sampler_desc, m_shadowMapSampler.ReleaseAndGetAddressOf());

}

void GameStageScene::CreateWindowSizeDependentResources()
{
	// �ˉe�s����쐬
	RECT rect = GetUserResources()->GetDeviceResources()->GetOutputSize();
	m_proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 200.0f
	);
}

void GameStageScene::OnDeviceLost()
{
	Finalize();
}

//// �Փ˔���̓o�^�i�f�o�b�O�p�j
//void GameStageScene::CollisionRegist()
//{
//	// --------------------------------------------------------------- //
//	// �Փ˔���̓o�^
//	// --------------------------------------------------------------- //
//	// ���{�b�g
//	m_displayCollision->AddBoundingOrientedBox(m_robotColl[0].GetBoundingOrientedBox()); // ���{�b�g�i�́j
//	m_displayCollision->AddBoundingOrientedBox(m_robotColl[1].GetBoundingOrientedBox()); // ���{�b�g�i�r�j
//	m_displayCollision->AddBoundingOrientedBox(m_robotColl[2].GetBoundingOrientedBox()); // ���{�b�g�i�E�r�j
//	m_displayCollision->AddBoundingOrientedBox(m_robotColl[3].GetBoundingOrientedBox()); // ���{�b�g�i���r�j
//	m_displayCollision->AddBoundingOrientedBox(m_robotColl[4].GetBoundingOrientedBox()); // ���{�b�g�i�~�T�C���j
//	// �e���f��
//	m_displayCollision->AddBoundingBox(m_modelColl[0].GetBoundingBox()); // ��
//	m_displayCollision->AddBoundingBox(m_modelColl[1].GetBoundingBox()); // WEC�ؔ�
//	m_displayCollision->AddBoundingBox(m_modelColl[2].GetBoundingBox()); // �ؔ�
//	m_displayCollision->AddBoundingBox(m_modelColl[3].GetBoundingBox()); // �˒I
//	m_displayCollision->AddBoundingBox(m_modelColl[4].GetBoundingBox()); // ���A��
//	m_displayCollision->AddBoundingBox(m_modelColl[5].GetBoundingBox()); // �x�b�h
//	m_displayCollision->AddBoundingBox(m_modelColl[6].GetBoundingBox()); // ��
//	m_displayCollision->AddBoundingBox(m_modelColl[7].GetBoundingBox()); // �����^��
//	// ��
//	m_displayCollision->AddBoundingOrientedBox(m_wallColl[0].GetBoundingOrientedBox());
//	m_displayCollision->AddBoundingOrientedBox(m_wallColl[1].GetBoundingOrientedBox());
//	m_displayCollision->AddBoundingOrientedBox(m_wallColl[2].GetBoundingOrientedBox());
//	m_displayCollision->AddBoundingOrientedBox(m_wallColl[3].GetBoundingOrientedBox());
//
//	//// �Փ˔���̕\��
//	//auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
//	//auto states = GetUserResources()->GetCommonStates();
//	//m_displayCollision->DrawCollision(context, states, m_view, m_proj);
//}

// OBB����֐�
bool GameStageScene::HitCheckOBB(
	const DirectX::BoundingOrientedBox& box1,
	const DirectX::BoundingOrientedBox& box2
)
{
	return box1.Intersects(box2);
}

// �e�̏������֐�
void GameStageScene::InitializeShadow(ID3D11Device* device, ID3D11DeviceContext* context)
{
	// �x�[�V�b�N�G�t�F�N�g�̍쐬
	m_basicEffect = std::make_unique<BasicEffect>(device);
	// ���C�e�B���O(OFF)
	m_basicEffect->SetLightingEnabled(false);
	// ���_�J���[(OFF)
	m_basicEffect->SetVertexColorEnabled(false);
	// �e�N�X�`��(ON)
	m_basicEffect->SetTextureEnabled(true);

	// ���̓��C�A�E�g�̍쐬
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionTexture>(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf()
		)
	);

	// �v���~�e�B�u�o�b�`�̍쐬
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	// �e�̃e�N�X�`���̓ǂݍ���
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(
			device,
			L"Resources/Textures/Shadow.dds",
			nullptr,
			m_shadowTexture.ReleaseAndGetAddressOf()
		)
	);
}

// �e�̏I������
void GameStageScene::ResetShadow()
{
	m_basicEffect.reset();
	m_primitiveBatch.reset();
}

// �e�̕`��֐�
void GameStageScene::DrawShadow(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	DirectX::SimpleMath::Vector3 position,
	float radius
)
{
	// �G�t�F�N�g�̐ݒ聕�K��
	m_basicEffect->SetWorld(SimpleMath::Matrix::Identity);
	m_basicEffect->SetView(m_view);
	m_basicEffect->SetProjection(m_proj);
	m_basicEffect->SetTexture(m_shadowTexture.Get());
	m_basicEffect->Apply(context);

	// ���̓��C�A�E�g�̐ݒ�
	context->IASetInputLayout(m_inputLayout.Get());

	// �e�N�X�`���T���v���̐ݒ�
	ID3D11SamplerState* sampler[] = { states->LinearClamp() };
	context->PSSetSamplers(0, 1, sampler);

	// �A���t�@�u�����h�̐ݒ�
	context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xffffffff);

	// �[�x�o�b�t�@�̐ݒ�
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	// �[�x�X�e���V���̐ݒ�
	context->OMSetDepthStencilState(m_depthStencilState_Shadow.Get(), 1);

	// �e�̒��_���
	VertexPositionTexture vertexes[] =
	{
		VertexPositionTexture(SimpleMath::Vector3::Zero, SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3::Zero, SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3::Zero, SimpleMath::Vector2(0.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3::Zero, SimpleMath::Vector2(1.0f, 1.0f)),
	};
	// �e�̃C���f�b�N�X���
	uint16_t indexes[] = { 0, 1, 2, 1, 3, 2 };

	// �e�̕\���ʒu�̐ݒ�
	vertexes[0].position = SimpleMath::Vector3(-radius, 0.01f, -radius) + position;
	vertexes[1].position = SimpleMath::Vector3(radius, 0.01f, -radius) + position;
	vertexes[2].position = SimpleMath::Vector3(-radius, 0.01f, radius) + position;
	vertexes[3].position = SimpleMath::Vector3(radius, 0.01f, radius) + position;

	// �e�̕`��
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawIndexed(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		indexes, _countof(indexes),
		vertexes, _countof(vertexes)
	);
	m_primitiveBatch->End();
}
