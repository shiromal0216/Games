//--------------------------------------------------------------------------------------
// File: ShadowMap.h
//
// �V���h�E�}�b�v�N���X
//
// Date: 2023.9.4
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "SimpleMath.h"
#include "CommonStates.h"

namespace Imase
{
	class ShadowMap
	{
	private:

		// �V���h�E�}�b�v�̉𑜓x
		static const UINT DEFALT_RESOLUTION = 2048;

		// �[�x�o�C�A�X�i�����l 0.1 �o�C�A�X�l�̌v�Z�� m_offset / ���C�g�̉e���͈́j
		static const float DEFALT_OFFSET;

		// �V���h�E�}�b�v�쐬���̃j�A�[�N���b�v�̒l
		static const float NEAR_CLIP;

		// cso�̏ꏊ
		std::wstring m_path;

		// �V���h�E�}�b�v�̉𑜓x
		UINT m_resolution;

		// �K�E�V�A���t�B���^
		bool m_gaussianFilterEnable;

		// ���U�V���h�E
		bool m_vsmFilterEnable;

		// �[�x�I�t�Z�b�g
		float m_offset;

		// ���_�V�F�[�_�[
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS_ShadowMap;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS_ShadowModel;

		// �s�N�Z���V�F�[�_�[
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS_ShadowMap;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS_ShadowModel;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS_ShadowModel_Texture;

		// �[�x�o�b�t�@�p�e�N�X�`��
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depathTexture2D;

		// �V���h�E�}�b�v�p�e�N�X�`��
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_shadowMapTexture2D;

		// �[�x�o�b�t�@�r���[
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DSV;

		// �V���h�E�}�b�v�p�̃����_�[�^�[�Q�b�g�r���[
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_shadowMapRTV;

		// �V���h�E�}�b�v�p�̃V�F�[�_�[���\�[�X�r���[
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shadowMapSRV;

		// �V���h�E�}�b�v�p�̓��̓��C�A�̓��̓��C�A�E�g�E�g
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_shadowMapInputLayout;

		// �r���[�|�[�g�y0:�V���h�E�}�b�v�p�z
		D3D11_VIEWPORT m_viewPort;

		// �萔�o�b�t�@�̍\���̂̒�`
		struct ConstantBuffer
		{
			DirectX::XMMATRIX lightViewProj;
			DirectX::XMFLOAT3 lightPosition;
			float maxDepth;
			float offset;
			float VSMFilterEnable;
			float test;
		};

		// �萔�o�b�t�@
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_shadowMapConstantBuffer;

		// �T���v���[
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler;

		// ���_�o�b�t�@
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBufferGF;

	private:

		// cso�̓����Ă���f�B���N�g�̐ݒ�֐�
		void SetDirectory(const wchar_t* path);

		// �p�X�t���̖��O�ɕϊ�����֐�
		const wchar_t* GetFullName(const wchar_t* name)
		{
			static std::wstring fullName;

			fullName = m_path;
			fullName += name;

			return fullName.c_str();
		}

		// ������x�N�g�����Z�o����֐�
		DirectX::SimpleMath::Vector3 CalcUpVector(const DirectX::SimpleMath::Vector3& v);

	public:

		// �R���X�g���N�^
		ShadowMap();

		// �������֐�
		void Initialize(ID3D11Device* device, const wchar_t* path = nullptr, UINT resolution = DEFALT_RESOLUTION);

		// �V���h�E�}�b�v�쐬�J�n
		// maxDepth�̒l�̓��C�g�̈ʒu���烉�C�g�̉e�����󂯂�͈�
		void Begin(ID3D11DeviceContext* context, DirectX::SimpleMath::Vector3 lightPos, DirectX::SimpleMath::Vector3 targetPos, float maxDepth);

		// �V���h�E�}�b�v�쐬���ɌĂяo���J�X�^���X�e�[�g
		void DrawShadowMap(ID3D11DeviceContext* context);

		// �e�t���̃��f����`�悵�������ɌĂяo���J�X�^���X�e�[�g
		// �e�N�X�`���Ȃ��̃��f����\���������ꍇ�͑�Q������false�ɂ��鎖
		void DrawShadow(ID3D11DeviceContext* context, bool texture);

		// �V���h�E�}�b�v�p�e�N�X�`�����\�[�X���擾����֐��i�f�o�b�O�p�j
		ID3D11ShaderResourceView* GetShadowMapTexture()
		{
			return m_shadowMapSRV.Get();
		}

		// �[�x�o�b�t�@�̔�r�I�t�Z�b�g�l�̐ݒ�i�ڂ������ꍇ�̓I�t�Z�b�g�l�͖����j
		void SetOffset(float offset)
		{
			m_offset = offset;
		}

	};
}

