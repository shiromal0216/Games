#include "pch.h"
#include "TitleScene.h"
#include "DebugDraw.h"
#include "ReadData.h"
#include <iomanip>
#include "StageSelectScene.h"
#include "GameStageScene.h"

using namespace DirectX;

TitleScene::TitleScene()
{
	// �X�|�b�g���C�g�͈̔͂̊p�x
	m_lightTheta = 45.0f;

	// ���C�g�̈ʒu
	m_lightPosition = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
}

void TitleScene::Initialize()
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
	m_parts[RIGHT] = std::make_unique<Imase::ModelPart>(m_rightModel.get());
	m_parts[MISSILE] = std::make_unique<Imase::ModelPart>(m_missileModel.get());

	// ���{�b�g�̊e�p�[�c��A������
	m_parts[ROOT]->SetChild(m_parts[LEG].get());
	m_parts[LEG]->SetChild(m_parts[BODY].get());
	m_parts[BODY]->SetChild(m_parts[HEAD].get());
	m_parts[HEAD]->SetSibling(m_parts[ARM_R].get());
	m_parts[ARM_R]->SetSibling(m_parts[ARM_L].get());
	m_parts[ARM_R]->SetChild(m_parts[MISSILE].get());
	m_parts[ARM_L]->SetChild(m_parts[RIGHT].get());

	// �e�p�[�c�̏����ʒu�s����쐬����
	m_parts[BODY]->SetInitialMatrix(
		SimpleMath::Matrix::CreateTranslation(0.0f, 0.32f, 0.0f));
	m_parts[HEAD]->SetInitialMatrix(
		SimpleMath::Matrix::CreateTranslation(0.0f, 0.75f, 0.0f));
	m_parts[ARM_R]->SetInitialMatrix(
		SimpleMath::Matrix::CreateTranslation(-0.18f, 0.72f, 0.0f));
	//���r�Ɉʒu���
	m_leftArmPosition = SimpleMath::Vector3(0.18f, 0.72f, 0.0f);
	m_parts[ARM_L]->SetInitialMatrix(
		SimpleMath::Matrix::CreateTranslation(m_leftArmPosition));
	m_parts[RIGHT]->SetInitialMatrix(
		SimpleMath::Matrix::CreateTranslation(0.28f, 0.52f, 0.39f));
	m_parts[MISSILE]->SetInitialMatrix(
		SimpleMath::Matrix::CreateTranslation(-0.28f, 0.52f, 0.39f));

	// �A������Ă���p�[�c�̏����ʒu�s���e�̍��W�n�ɑS�ĕϊ�����
	m_parts[ROOT]->SetupMatrix();

	// �萔�o�b�t�@�̓��e�X�V
	ConstantBuffer2 cb = {};
	cb.fCosTheta = cosf(XMConvertToRadians(m_lightTheta / 2.0f));
	context->UpdateSubresource(m_constantBuffer2.Get(), 0, nullptr, &cb, 0, 0);
}

void TitleScene::Update(float elapsedTime)
{
	elapsedTime;

	// ���̃V�[����
	auto transitionMask = GetUserResources()->GetTransitionMask();
	auto kbTracker = GetUserResources()->GetKeyboardStateTracker();
	auto kb = kbTracker->GetLastState();

	// �f�o�b�O�J�����̍X�V
	m_debugCamera->Update();

	// �J������]
	m_camera.SetType(GameCamera::Type::Type_B);

	// ���{�b�g�̈ړ�
	m_robotPosition += SimpleMath::Vector3::Transform(SimpleMath::Vector3(0.0f, 0.0f, 0.01f), m_robotRotate);

	//���C�g�����{�b�g�̘r�Ɉړ�������
	m_lightPosition = m_robotPosition + m_leftArmPosition;

	// �J�����̍X�V
	m_camera.Update(elapsedTime);

	// -------------------------------------------- //
	// �V�[���؂�ւ�
	// -------------------------------------------- //
	// �I�[�v�����Ă���Enter�L�[�������ꂽ
	if (transitionMask->IsOpen() && kbTracker->pressed.Enter)
	{
		// �N���[�Y����
		transitionMask->Close();
	}
	// �N���[�Y������ʂ̃V�[����
	if (transitionMask->IsClose() && transitionMask->IsEnd())
	{
		ChangeScene <StageSelectScene>();
	}
}

void TitleScene::Render()
{
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = GetUserResources()->GetCommonStates();

	// -------------------------------------------------------------------------- //
	// Pass1 �V���h�E�}�b�v�̍쐬
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
	cb.lightAmbient = SimpleMath::Color(0.2f, 0.2f, 0.2f);	// �X�e�[�W�̖��邳����

	*static_cast<ConstantBuffer*>(mappedResource.pData) = cb;

	// GPU���萔�o�b�t�@�ɑ΂��ẴA�N�Z�X��������
	context->Unmap(m_constantBuffer.Get(), 0);

	// -------------------------------------------------------------------------- //
	// �����_�[�^�[�Q�b�g�ƃr���[�|�[�g�����ɖ߂�
	// -------------------------------------------------------------------------- //
	auto renderTarget = GetUserResources()->GetDeviceResources()->GetRenderTargetView();
	auto depthStencil = GetUserResources()->GetDeviceResources()->GetDepthStencilView();

	context->ClearRenderTargetView(renderTarget, Colors::Black);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);
	auto const viewport = GetUserResources()->GetDeviceResources()->GetScreenViewport();
	context->RSSetViewports(1, &viewport);
	
	SimpleMath::Matrix world;								// �����̑傫��
	world = SimpleMath::Matrix::CreateScale(SimpleMath::Vector3(10.0f,1.0f,10.0f))
		* SimpleMath::Matrix::CreateTranslation(0.0f, 0.0f, 0.0f);

	// ���̕`��
	m_floorModel->Draw(context, *states, world, m_view, m_proj, false, [&]()
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

	// ���\�[�X�̊��蓖�Ă���������ishadowMapRT�j
	ID3D11ShaderResourceView* nullsrv[] = { nullptr };
	context->PSSetShaderResources(1, 1, nullsrv);

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

	m_spriteBatch->Begin();

	// �^�C�g�����S�摜�̕`��
	m_spriteBatch->Draw(m_titleSRV.Get(), SimpleMath::Vector2(TITLE_POSITION));

	// Press to start�摜�̕`��
	m_spriteBatch->Draw(m_startSRV.Get(), SimpleMath::Vector2(START_POSITION));

	m_spriteBatch->End();
}

void TitleScene::Finalize()
{
	m_headModel.reset();
	m_bodyModel.reset();
	m_legModel.reset();
	m_armRModel.reset();
	m_armLModel.reset();
	m_rightModel.reset();
	m_missileModel.reset();

	m_floorModel.reset();
	m_spriteBatch.reset();

	m_titleSRV.Reset();
	m_startSRV.Reset();

	// �e�̏I������
	ResetShadow();
}

void TitleScene::CreateDeviceDependentResources()
{
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();

	// �G�t�F�N�g�t�@�N�g���[�̍쐬
	EffectFactory fx(device);
	fx.SetDirectory(L"Resources/Models");

	std::unique_ptr<EffectFactory> fxrbt = std::make_unique<EffectFactory>(device);
	fxrbt->SetDirectory(L"Resources/Models");

	// ���{�b�g�̊e�p�[�c���f���̍쐬
	m_headModel = Model::CreateFromCMO(device, L"Resources/Models/Head.cmo", *fxrbt);
	m_bodyModel = Model::CreateFromCMO(device, L"Resources/Models/Body.cmo", *fxrbt);
	m_legModel = Model::CreateFromCMO(device, L"Resources/Models/Leg.cmo", *fxrbt);
	m_armRModel = Model::CreateFromCMO(device, L"Resources/Models/Arm_R.cmo", *fxrbt);
	m_armLModel = Model::CreateFromCMO(device, L"Resources/Models/Arm_L.cmo", *fxrbt);
	m_rightModel = Model::CreateFromCMO(device, L"Resources/Models/SpotLight.cmo", *fxrbt);
	m_missileModel = Model::CreateFromCMO(device, L"Resources/Models/Missile.cmo", *fxrbt);

	// �e�̏������֐�
	InitializeShadow(device, context);

	// ���̃��f���̍쐬
	m_floorModel = Model::CreateFromCMO(device, L"Resources/Models/Floor.cmo", fx);

	// �X�|�b�g���C�g�̃��f���̍쐬
	m_spotLightModel = Model::CreateFromCMO(device, L"Resources/Models/SpotLight.cmo", fx);

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

	// �^�C�g�����S�摜�̓ǂݍ���
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/Title.dds", nullptr, 
			m_titleSRV.ReleaseAndGetAddressOf())
	);

	// Press to start�摜�̓ǂݍ���
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/Press to start.dds", nullptr,
			m_startSRV.ReleaseAndGetAddressOf())
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

void TitleScene::CreateWindowSizeDependentResources()
{
	// �ˉe�s����쐬
	RECT rect = GetUserResources()->GetDeviceResources()->GetOutputSize();
	m_proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 200.0f
	);
}

void TitleScene::OnDeviceLost()
{
	Finalize();
}

// �e�̏������֐�
void TitleScene::InitializeShadow(ID3D11Device* device, ID3D11DeviceContext* context)
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
void TitleScene::ResetShadow()
{
	m_basicEffect.reset();
	m_primitiveBatch.reset();
}

// �e�̕`��֐�
void TitleScene::DrawShadow(
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
