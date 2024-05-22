//--------------------------------------------------------------------------------------
// File: ModelPart.h
//
// ���f���̃p�[�c�\���N���X
//
// Date: 2023.7.9
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

namespace Imase
{
	// ���f���p�[�c�N���X
	class ModelPart
	{
	protected:

		// ���f���n���h��
		DirectX::Model* m_model;

		// �q�ւ̃|�C���^
		ModelPart* m_child;

		// �Z��ւ̃|�C���^
		ModelPart* m_sibling;

		// �����ʒu�s��
		DirectX::SimpleMath::Matrix m_initial;

		// �ϊ��s��
		DirectX::SimpleMath::Matrix m_transform;

		// ���[���h�s��
		DirectX::SimpleMath::Matrix m_world;

		// �`��֐�
		void Draw(
			ModelPart* parts,
			ID3D11DeviceContext* context,
			const DirectX::CommonStates& states,
			DirectX::SimpleMath::Matrix view,
			DirectX::SimpleMath::Matrix proj,
			_In_opt_ std::function<void __cdecl()> setCustomState = nullptr
		)
		{
			if (parts->m_child) Draw(parts->m_child, context, states, view, proj, setCustomState);
			if (parts->m_sibling) Draw(parts->m_sibling, context, states, view, proj, setCustomState);
			if (parts->m_model) parts->m_model->Draw(context, states, parts->m_world, view, proj, false, setCustomState);
		}

		// �e�p�[�c�̏����ʒu�s���e�̍��W�n�ɕϊ�����֐�
		void SetupMatrix(ModelPart* parts, const DirectX::SimpleMath::Matrix& parent)
		{
			if (parts->m_child) SetupMatrix(parts->m_child, parts->m_initial.Invert());
			if (parts->m_sibling) SetupMatrix(parts->m_sibling, parent);
			parts->m_initial *= parent;
		}

		// �e�p�[�c�̃��[���h�s����X�V����֐�
		void UpdateMatrix(ModelPart* parts, const DirectX::SimpleMath::Matrix& parent)
		{
			parts->m_world = parts->m_transform * parts->m_initial;
			parts->m_world *= parent;
			if (parts->m_child) UpdateMatrix(parts->m_child, parts->m_world);
			if (parts->m_sibling) UpdateMatrix(parts->m_sibling, parent);
		}

	public:

		// �R���X�g���N�^
		ModelPart(DirectX::Model* model = nullptr)
			: m_model(model), m_child(nullptr), m_sibling(nullptr)
		{
		}

		// �f�X�g���N�^
		virtual ~ModelPart() {}

		// �q�p�[�c�̐ݒ�֐�
		void SetChild(ModelPart* part) { m_child = part; }

		// �Z��p�[�c�̐ݒ�֐�
		void SetSibling(ModelPart* part) { m_sibling = part; }

		// �����ʒu�s��̐ݒ�֐�
		void SetInitialMatrix(DirectX::SimpleMath::Matrix inital) { m_initial = inital; }

		// �ϊ��s��̐ݒ�֐�
		void SetTransformMatrix(DirectX::SimpleMath::Matrix transform) { m_transform = transform; }

		// �A������Ă���p�[�c�̏����ʒu�s���e�̍��W�n�ɑS�ĕϊ�����֐�
		void SetupMatrix() { SetupMatrix(this, DirectX::SimpleMath::Matrix::Identity); }

		// �A������Ă���p�[�c�̃��[���h�s���S�čX�V����֐�
		void UpdateMatrix() { UpdateMatrix(this, DirectX::SimpleMath::Matrix::Identity); }

		// �`��֐�
		void Draw(
			ID3D11DeviceContext* context,
			const DirectX::CommonStates& states,
			DirectX::SimpleMath::Matrix view,
			DirectX::SimpleMath::Matrix proj,
			_In_opt_ std::function<void __cdecl()> setCustomState = nullptr
		)
		{
			Draw(this, context, states, view, proj, setCustomState);
		}

		// ���[���h�s����擾����֐�
		const DirectX::SimpleMath::Matrix& GetWorldMatrix() { return m_world; }

		// ���f���ւ̃|�C���^���擾����֐�
		DirectX::Model* GetModel() { return m_model; }

	};
}

