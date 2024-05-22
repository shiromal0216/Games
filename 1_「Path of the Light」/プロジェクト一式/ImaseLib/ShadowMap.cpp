//--------------------------------------------------------------------------------------
// File: ShadowMap.cpp
//
// �V���h�E�}�b�v�N���X
//
// Date: 2022.6.19
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "ShadowMap.h"
#include "VertexTypes.h"
#include "ReadData.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Imase;

// �[�x�o�C�A�X�i�����l 0.1 �o�C�A�X�l�̌v�Z�� m_offset / ���C�g�̉e���͈́j
const float ShadowMap::DEFALT_OFFSET = 0.2f;

// �V���h�E�}�b�v�쐬���̃j�A�[�N���b�v�̒l
const float ShadowMap::NEAR_CLIP = 0.1f;

ShadowMap::ShadowMap()
	: m_gaussianFilterEnable(true)
	, m_vsmFilterEnable(true)
	, m_resolution(ShadowMap::DEFALT_RESOLUTION)
	, m_viewPort{}
	, m_offset(ShadowMap::DEFALT_OFFSET)
	, m_path{}
{
}

// �������֐�
void ShadowMap::Initialize(ID3D11Device* device, const wchar_t* path, UINT resolution)
{
	SetDirectory(path);

	// �V���h�E�}�b�v�̉𑜓x
	m_resolution = resolution;

	// �V�F�[�_�[�̓ǂݍ���
	std::vector<uint8_t> vs_ShadowMap = DX::ReadData(GetFullName(L"SM_VS_Depth.cso"));
	std::vector<uint8_t> ps_ShadowMap = DX::ReadData(GetFullName(L"SM_PS_Depth.cso"));
	std::vector<uint8_t> vs_ShadowModel = DX::ReadData(GetFullName(L"SM_VS.cso"));
	std::vector<uint8_t> ps_ShadowModel = DX::ReadData(GetFullName(L"SM_PS.cso"));
	std::vector<uint8_t> ps_ShadowModel_Tex = DX::ReadData(GetFullName(L"SM_PS_Texture.cso"));

	// �V�F�[�_�[�̍쐬
	device->CreateVertexShader(vs_ShadowMap.data(), vs_ShadowMap.size(), nullptr, m_VS_ShadowMap.ReleaseAndGetAddressOf());
	device->CreatePixelShader(ps_ShadowMap.data(), ps_ShadowMap.size(), nullptr, m_PS_ShadowMap.ReleaseAndGetAddressOf());
	device->CreateVertexShader(vs_ShadowModel.data(), vs_ShadowModel.size(), nullptr, m_VS_ShadowModel.ReleaseAndGetAddressOf());
	device->CreatePixelShader(ps_ShadowModel.data(), ps_ShadowModel.size(), nullptr, m_PS_ShadowModel.ReleaseAndGetAddressOf());
	device->CreatePixelShader(ps_ShadowModel_Tex.data(), ps_ShadowModel_Tex.size(), nullptr, m_PS_ShadowModel_Texture.ReleaseAndGetAddressOf());

	// ���̓��C�A�E�g�̍쐬
	DX::ThrowIfFailed(
		device->CreateInputLayout(
			VertexPositionNormalTangentColorTexture::InputElements, VertexPositionNormalTangentColorTexture::InputElementCount,
			vs_ShadowModel.data(), vs_ShadowModel.size(),
			m_shadowMapInputLayout.GetAddressOf()
		)
	);

	{
		// �e�N�X�`���̍쐬�i�[�x�o�b�t�@�p�j
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = m_resolution;   // ��
		desc.Height = m_resolution;  // ����
		desc.MipLevels = 1;       // �~�b�v�}�b�v ���x����
		desc.ArraySize = 1;       // �z��T�C�Y
		desc.Format = DXGI_FORMAT_D16_UNORM;  // �t�H�[�}�b�g
		desc.SampleDesc.Count = 1;  // �}���`�T���v�����O�̐ݒ�
		desc.SampleDesc.Quality = 0;  // �}���`�T���v�����O�̕i��
		desc.Usage = D3D11_USAGE_DEFAULT;      // �f�t�H���g�g�p�@
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL; // �[�x�o�b�t�@�Ƃ��Ďg�p
		desc.CPUAccessFlags = 0;   // CPU����̓A�N�Z�X���Ȃ�
		desc.MiscFlags = 0;   // ���̑��̐ݒ�Ȃ�
		DX::ThrowIfFailed(device->CreateTexture2D(&desc, nullptr, m_depathTexture2D.GetAddressOf()));
	}

	{
		// �e�N�X�`���̍쐬�i�V���h�E�}�b�v�쐬�p�j
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = m_resolution;   // ��
		desc.Height = m_resolution;  // ����
		desc.MipLevels = 1;       // �~�b�v�}�b�v ���x����
		desc.ArraySize = 1;       // �z��T�C�Y
		desc.Format = DXGI_FORMAT_R16G16_FLOAT;  // �t�H�[�}�b�g
		desc.SampleDesc.Count = 1;  // �}���`�T���v�����O�̐ݒ�
		desc.SampleDesc.Quality = 0;  // �}���`�T���v�����O�̕i��
		desc.Usage = D3D11_USAGE_DEFAULT;      // �f�t�H���g�g�p�@
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; // �[�x/�X�e���V���A�V�F�[�_ ���\�[�X �r���[�Ƃ��Ďg�p
		desc.CPUAccessFlags = 0;   // CPU����̓A�N�Z�X���Ȃ�
		desc.MiscFlags = 0;   // ���̑��̐ݒ�Ȃ�
		// �[�x�������ݗp
		DX::ThrowIfFailed(device->CreateTexture2D(&desc, nullptr, m_shadowMapTexture2D.GetAddressOf()));

	}

	{
		// �[�x/�X�e���V�� �r���[�̍쐬
		D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_D16_UNORM;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Flags = 0;
		desc.Texture2D.MipSlice = 0;
		DX::ThrowIfFailed(device->CreateDepthStencilView(m_depathTexture2D.Get(), &desc, m_DSV.GetAddressOf()));
	}

	{
		// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
		D3D11_RENDER_TARGET_VIEW_DESC desc;
		desc.Format = DXGI_FORMAT_R16G16_FLOAT;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;
		DX::ThrowIfFailed(device->CreateRenderTargetView(m_shadowMapTexture2D.Get(), &desc, m_shadowMapRTV.GetAddressOf()));
	}

	{
		// �V�F�[�_���\�[�X�r���[�̍쐬
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		desc.Format = DXGI_FORMAT_R16G16_FLOAT; // �t�H�[�}�b�g
		desc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2D�e�N�X�`��
		desc.Texture2D.MostDetailedMip = 0;   // �ŏ��̃~�b�v�}�b�v ���x��
		desc.Texture2D.MipLevels = static_cast<UINT>(-1);  // ���ׂẴ~�b�v�}�b�v ���x��
		DX::ThrowIfFailed(device->CreateShaderResourceView(m_shadowMapTexture2D.Get(), &desc, m_shadowMapSRV.GetAddressOf()));
	}

	// �r���[�|�[�g
	m_viewPort.TopLeftX = 0.0f;		// �r���[�|�[�g�̈�̍���X���W�B
	m_viewPort.TopLeftY = 0.0f;		// �r���[�|�[�g�̈�̍���Y���W�B
	m_viewPort.Width = static_cast<FLOAT>(m_resolution);	// �r���[�|�[�g�̈�̕�
	m_viewPort.Height = static_cast<FLOAT>(m_resolution);	// �r���[�|�[�g�̈�̍���
	m_viewPort.MinDepth = 0.0f;		// �r���[�|�[�g�̈�̐[�x�l�̍ŏ��l
	m_viewPort.MaxDepth = 1.0f;		// �r���[�|�[�g�̈�̐[�x�l�̍ő�l

	// �萔�o�b�t�@�̍쐬
	{
		D3D11_BUFFER_DESC desc = {};
		// �o�b�t�@�T�C�Y�͂P�U�̔{���łȂ��Ƃ����Ȃ�
		size_t size = sizeof(ConstantBuffer);
		if (size % 16) size++;
		desc.ByteWidth = static_cast<UINT>(size * 16);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		DX::ThrowIfFailed(device->CreateBuffer(&desc, nullptr, m_shadowMapConstantBuffer.GetAddressOf()));
	}

	{
		// �V���h�E�}�b�v�p�T���v���[�̍쐬
		D3D11_SAMPLER_DESC desc;
		desc.Filter = D3D11_FILTER_ANISOTROPIC;
		desc.MipLODBias = 0.0f;
		desc.MaxAnisotropy = 2;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MinLOD = -FLT_MAX;
		desc.MaxLOD = FLT_MAX;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.BorderColor[0] = 1.0f;
		desc.BorderColor[1] = 1.0f;
		desc.BorderColor[2] = 1.0f;
		desc.BorderColor[3] = 1.0f;
		DX::ThrowIfFailed(device->CreateSamplerState(&desc, m_sampler.GetAddressOf()));
	}

}

void ShadowMap::Begin(
	ID3D11DeviceContext* context,
	DirectX::SimpleMath::Vector3 lightPos,
	DirectX::SimpleMath::Vector3 targetPos,
	float maxDepth
)
{
	// �f�o�C�X�R���e�L�X�g�̃N���A
	context->ClearState();

	// �[�x�o�b�t�@���N���A
	context->ClearDepthStencilView(m_DSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	// �����_�[�^�[�Q�b�g���N���A
	context->ClearRenderTargetView(m_shadowMapRTV.Get(), Colors::White);

	// �����_�[�^�[�Q�b�g��ݒ�
	context->OMSetRenderTargets(1, m_shadowMapRTV.GetAddressOf(), m_DSV.Get());

	// �r���[�|�[�g�̐ݒ�
	context->RSSetViewports(1, &m_viewPort);

	//----------------------------------//
	// �萔�o�b�t�@��ݒ�               //
	//----------------------------------//
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		DX::ThrowIfFailed(
			context->Map(m_shadowMapConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)
		);

		ConstantBuffer buffer = {};

		Vector3 vec = targetPos - lightPos;
		Matrix lightView = Matrix::CreateLookAt(lightPos, targetPos, CalcUpVector(vec));
		Matrix projection = Matrix::CreatePerspectiveFieldOfView(XMConvertToRadians(45.0f), 1.0f, NEAR_CLIP, NEAR_CLIP + maxDepth);
		Matrix lightViewProj = lightView * projection;
		buffer.lightViewProj = XMMatrixTranspose(lightViewProj);
		buffer.lightPosition = lightPos;
		buffer.maxDepth = maxDepth;
		buffer.offset = m_offset / maxDepth;
		//		buffer.VSMFilterEnable = m_vsmFilterEnable;
		buffer.VSMFilterEnable = false;

		*static_cast<ConstantBuffer*>(mappedResource.pData) = buffer;
		context->Unmap(m_shadowMapConstantBuffer.Get(), 0);
	}
	//----------------------------------//
	// �萔�o�b�t�@��ݒ�i�I�j         //
	//----------------------------------//
}

// �V���h�E�}�b�v�쐬���ɌĂяo���J�X�^���X�e�[�g
void ShadowMap::DrawShadowMap(ID3D11DeviceContext* context)
{
	// ���̓��C�A�E�g�̐ݒ�
	context->IASetInputLayout(m_shadowMapInputLayout.Get());

	// ���_�V�F�[�_�[�̐ݒ�
	context->VSSetShader(m_VS_ShadowMap.Get(), nullptr, 0);
	context->VSSetConstantBuffers(1, 1, m_shadowMapConstantBuffer.GetAddressOf());

	// �s�N�Z���V�F�[�_�[�̐ݒ�
	context->PSSetShader(m_PS_ShadowMap.Get(), nullptr, 0);
	context->PSSetConstantBuffers(1, 1, m_shadowMapConstantBuffer.GetAddressOf());
}

// �e�t���̃��f����`�悵�������ɌĂяo���J�X�^���X�e�[�g
// �e�N�X�`���Ȃ��̃��f����\���������ꍇ�͑�Q������false�ɂ��鎖
void ShadowMap::DrawShadow(ID3D11DeviceContext* context, bool texture)
{
	// ���̓��C�A�E�g�̐ݒ�
	context->IASetInputLayout(m_shadowMapInputLayout.Get());

	// ���_�V�F�[�_�[�̐ݒ�
	context->VSSetShader(m_VS_ShadowModel.Get(), nullptr, 0);
	context->VSSetConstantBuffers(1, 1, m_shadowMapConstantBuffer.GetAddressOf());

	// �s�N�Z���V�F�[�_�[�̐ݒ�
	if (texture)
	{
		// �e�N�X�`���L
		context->PSSetShader(m_PS_ShadowModel_Texture.Get(), nullptr, 0);
	}
	else
	{
		// �e�N�X�`���Ȃ�
		context->PSSetShader(m_PS_ShadowModel.Get(), nullptr, 0);
	}
	context->PSSetConstantBuffers(1, 1, m_shadowMapConstantBuffer.GetAddressOf());
	context->PSSetSamplers(1, 1, m_sampler.GetAddressOf());

	// �V���h�E�}�b�v���V�F�_�[���\�[�X�r���[�֐ݒ肷��
	context->PSSetShaderResources(1, 1, m_shadowMapSRV.GetAddressOf());
}

// cso�̓����Ă���f�B���N�g�̐ݒ�֐�
void ShadowMap::SetDirectory(const wchar_t* path)
{
	if (path)
	{
		m_path = path;
		if (m_path[m_path.size() - 1] != L'/')
		{
			m_path += L"/";
		}
	}
	else
	{
		m_path.clear();
	}
}

// ������x�N�g�����Z�o����֐�
DirectX::SimpleMath::Vector3 ShadowMap::CalcUpVector(const DirectX::SimpleMath::Vector3& v)
{
	Matrix rotY = Matrix::CreateRotationY(atan2f(v.z, v.x));
	Vector3 tmp = Vector3::Transform(v, rotY);
	Vector3 up = Vector3(-tmp.y, tmp.x, tmp.z);
	up = Vector3::Transform(up, rotY.Invert());
	up.Normalize();
	return up;
}

