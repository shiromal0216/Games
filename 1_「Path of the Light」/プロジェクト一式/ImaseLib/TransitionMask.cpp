//--------------------------------------------------------------------------------------
// File: TransitionMask.cpp
//
// ��ʐ؂�ւ��p�̃}�X�N�\���N���X
//
// Date: 2023.9.10
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "TransitionMask.h"
#include "ReadData.h"
#include "CommonSys.h"

using namespace DirectX;

// �R���X�g���N�^
TransitionMask::TransitionMask(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	float interval
)
	: m_interval(interval)
	, m_rate(0.0f)
	, m_open(true)
	, m_request(CreateMaskRequest::NONE)
{
	// �X�v���C�g�o�b�`�̍쐬
	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	// �s�N�Z���V�F�[�_�[�̍쐬
	std::vector<uint8_t> ps_fade = DX::ReadData(L"Resources/Shaders/PS_Fade.cso");
	DX::ThrowIfFailed(
		device->CreatePixelShader(ps_fade.data(), ps_fade.size(), nullptr, m_PS_Fade.ReleaseAndGetAddressOf())
	);

	// �萔�o�b�t�@�̍쐬
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = static_cast<UINT>(sizeof(ConstantBuffer));	// �m�ۂ���T�C�Y�i16�̔{���Őݒ肷��j
	// GPU (�ǂݎ���p) �� CPU (�������ݐ�p) �̗�������A�N�Z�X�ł��郊�\�[�X
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// �萔�o�b�t�@�Ƃ��Ĉ���
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPU�����e��ύX�ł���悤�ɂ���
	DX::ThrowIfFailed(device->CreateBuffer(&bufferDesc, nullptr, m_constantBuffer.ReleaseAndGetAddressOf()));

	// �}�X�N�p�e�N�X�`���̓ǂݍ���
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/FadeMask.dds", nullptr, m_maskTexture.ReleaseAndGetAddressOf())
	);
}

// �X�V����
void TransitionMask::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	if (m_open)
	{
		// �I�[�v��
		m_rate -= FRAME_TIME / m_interval;
		if (m_rate < 0.0f) m_rate = 0.0f;
	}
	else
	{
		// �N���[�Y
		m_rate += FRAME_TIME / m_interval;
		if (m_rate > 1.0f) m_rate = 1.0f;
	}
}

// �`�揈��
void TransitionMask::Draw(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	ID3D11ShaderResourceView* texture,
	const RECT& rect
)
{
	if (m_rate == 0.0f) return;

	// -------------------------------------------------------------------------- //
	// �萔�o�b�t�@���X�V
	// -------------------------------------------------------------------------- //

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	// GPU���萔�o�b�t�@�ɑ΂��ăA�N�Z�X���s��Ȃ��悤�ɂ���
	context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// �萔�o�b�t�@���X�V
	static_cast<ConstantBuffer*>(mappedResource.pData)->rate = m_rate;

	// GPU���萔�o�b�t�@�ɑ΂��ẴA�N�Z�X��������
	context->Unmap(m_constantBuffer.Get(), 0);

	m_spriteBatch->Begin(SpriteSortMode_Immediate, states->NonPremultiplied(), nullptr, nullptr, nullptr, [&]()
		{
			// �萔�o�b�t�@�̐ݒ�
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get() };
			context->PSSetConstantBuffers(1, 1, cbuf);

			// �}�X�N�p�e�N�X�`����ݒ�
			auto srv = m_maskTexture.Get();
			context->PSSetShaderResources(1, 1, &srv);

			// �V�F�[�_�[�̐ݒ�
			context->PSSetShader(m_PS_Fade.Get(), nullptr, 0);
		}
	);

	m_spriteBatch->Draw(texture, rect);

	m_spriteBatch->End();
}

// �}�X�N���I�[�v������֐�
void TransitionMask::Open()
{
	m_open = true;
	m_rate = 1.0f;
}

// �}�X�N���N���[�Y����֐�
void TransitionMask::Close()
{
	m_open = false;
	m_rate = 0.0f;
}

