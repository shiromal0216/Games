//--------------------------------------------------------------------------------------
// File: ModelCollision.h
//
// ���f���p�Փ˔���N���X
//
// Date: 2023.6.10
// Author: Hideyasu Imase
//
//--------------------------------------------------------------------------------------
#pragma once

#include "ImaseLib/DisplayCollision.h"

namespace Imase
{
	class ModelCollisionSphere;
	class ModelCollisionBox;
	class ModelCollisionOrientedBox;

	// �Փ˔���̊��N���X
	class ModelCollision
	{
	public:

		// �R���W�����̃^�C�v
		enum class CollisionType
		{
			None,
			Sphere,
			AABB,
			OBB,
		};

	private:

		// �R���W�����^�C�v
		CollisionType m_type;

	public:

		// �R���X�g���N�^
		ModelCollision(CollisionType type);

		// �f�X�g���N�^
		virtual ~ModelCollision() {}

		// �Փ˔�����s���֐�
		virtual bool Intersects(ModelCollision* collision) = 0;

		// �������s���֐�
		virtual bool Contains(ModelCollision* collision) = 0;

		// �R���W�����^�C�v�̎擾�֐�
		CollisionType GetType() { return m_type; }

		// �R���W�������̍X�V
		virtual void UpdateBoundingInfo(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion rotate) = 0;

		// �Փ˔���̕\���ɓo�^����֐�
		virtual void AddDisplayCollision(Imase::DisplayCollision* displayCollision) = 0;

	protected:

		// �Փ˔���i�������b�V���ɑΉ��j
		template <class T, class U>
		bool Intersects(T a, U b)
		{
			for (auto& data_a : a)
			{
				for (auto& data_b : b)
				{
					if (data_a.Intersects(data_b))
					{
						return true;
					}
				}
			}
			return false;
		}

		// �Փ˔���
		// T ���̊֐����Ăяo���N���X�̌^�iCollisionSphere�Ȃǁj
		// U �N���X��GetBoundingInfo�֐����Ԃ��^�iDirectX::BoundingSphere�Ȃǁj
		template <class T, class U>
		bool Intersects(T* a, ModelCollision* b)
		{
			if (b == nullptr) return false;

			switch (b->GetType())
			{
			case CollisionType::Sphere:
			{
				auto p = dynamic_cast<ModelCollisionSphere*>(b);
				assert(p);
				return ModelCollision::Intersects<const std::vector<U>&, const std::vector<DirectX::BoundingSphere>&>
					(a->GetBoundingInfo(), p->GetBoundingInfo());
			}
			case CollisionType::AABB:
			{
				auto p = dynamic_cast<ModelCollisionBox*>(b);
				assert(p);
				return ModelCollision::Intersects<const std::vector<U>&, const std::vector<DirectX::BoundingBox>&>
					(a->GetBoundingInfo(), p->GetBoundingInfo());
			}
			case CollisionType::OBB:
			{
				auto p = dynamic_cast<ModelCollisionOrientedBox*>(b);
				assert(p);
				return ModelCollision::Intersects<const std::vector<U>&, const std::vector<DirectX::BoundingOrientedBox>&>
					(a->GetBoundingInfo(), p->GetBoundingInfo());
			}
			default:
				break;
			}
			return false;
		}

		// �����
		template <class T, class U>
		bool Contains(T a, U b)
		{
			for (auto& data_a : a)
			{
				for (auto& data_b : b)
				{
					if (data_a.Contains(data_b) != DirectX::CONTAINS)
					{
						return false;
					}
				}
			}
			return true;
		}

		// �����
		// T ���̊֐����Ăяo���N���X�̌^�iCollisionSphere�Ȃǁj
		// U �N���X��GetBoundingInfo�֐����Ԃ��^�iDirectX::BoundingSphere�Ȃǁj
		template <class T, class U>
		bool Contains(T* a, ModelCollision* b)
		{
			if (b == nullptr) return false;

			switch (b->GetType())
			{
			case CollisionType::Sphere:
			{
				auto p = dynamic_cast<ModelCollisionSphere*>(b);
				assert(p);
				return ModelCollision::Contains<const std::vector<U>&, const std::vector<DirectX::BoundingSphere>&>
					(a->GetBoundingInfo(), p->GetBoundingInfo());
			}
			case CollisionType::AABB:
			{
				auto p = dynamic_cast<ModelCollisionBox*>(b);
				assert(p);
				return ModelCollision::Contains<const std::vector<U>&, const std::vector<DirectX::BoundingBox>&>
					(a->GetBoundingInfo(), p->GetBoundingInfo());
			}
			case CollisionType::OBB:
			{
				auto p = dynamic_cast<ModelCollisionOrientedBox*>(b);
				assert(p);
				return ModelCollision::Contains<const std::vector<U>&, const std::vector<DirectX::BoundingOrientedBox>&>
					(a->GetBoundingInfo(), p->GetBoundingInfo());
			}
			default:
				break;
			}
			return false;
		}

	};

	// �Փ˔���N���X�i���j
	class ModelCollisionSphere : public ModelCollision
	{
	private:

		// �Փ˔�����
		std::vector<DirectX::BoundingSphere*> m_srcSpheres;

		// �Փ˔�����i���[���h��ԁj
		std::vector<DirectX::BoundingSphere> m_dstSpheres;

	public:

		// �R���X�g���N�^
		ModelCollisionSphere(DirectX::Model* pModel);

		// �Փ˔�����̍X�V
		void UpdateBoundingInfo(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion rotate) override;

		// �Փ˔�����s���֐�
		bool Intersects(ModelCollision* collision) override;

		// �������s���֐�
		bool Contains(ModelCollision* collision) override;

		// �Փ˔�����擾�֐�
		const std::vector<DirectX::BoundingSphere>& GetBoundingInfo() { return m_dstSpheres; }

		// �Փ˔���̕\���ɓo�^����֐�
		void AddDisplayCollision(Imase::DisplayCollision* displayCollision);

	};

	// �Փ˔���N���X�iAABB�j
	class ModelCollisionBox : public ModelCollision
	{
	private:

		// �Փ˔�����
		std::vector<DirectX::BoundingBox*> m_srcBoxes;

		// �Փ˔�����i���[���h��ԁj
		std::vector<DirectX::BoundingBox> m_dstBoxes;

	public:

		// �R���X�g���N�^
		ModelCollisionBox(DirectX::Model* pModel);

		// �Փ˔�����̍X�V
		void UpdateBoundingInfo(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion rotate) override;

		// �Փ˔�����s���֐�
		bool Intersects(ModelCollision* collision) override;

		// �������s���֐�
		bool Contains(ModelCollision* collision) override;

		// �Փ˔�����擾�֐�
		const std::vector<DirectX::BoundingBox>& GetBoundingInfo() { return m_dstBoxes; }

		// �Փ˔���̕\���ɓo�^����֐�
		void AddDisplayCollision(Imase::DisplayCollision* displayCollision);

	};

	// �Փ˔���N���X�iOBB�j
	class ModelCollisionOrientedBox : public ModelCollision
	{
	private:

		// �Փ˔�����
		std::vector<DirectX::BoundingBox*> m_srcBoxes;

		// �Փ˔�����i���[���h��ԁj
		std::vector<DirectX::BoundingOrientedBox> m_dstOrientedBoxes;

	public:

		// �R���X�g���N�^
		ModelCollisionOrientedBox(DirectX::Model* pModel);

		// �Փ˔�����̍X�V
		void UpdateBoundingInfo(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion rotate) override;

		// �Փ˔�����s���֐�
		bool Intersects(ModelCollision* collision) override;

		// �������s���֐�
		bool Contains(ModelCollision* collision) override;

		// �Փ˔�����擾�֐�
		const std::vector<DirectX::BoundingOrientedBox>& GetBoundingInfo() { return m_dstOrientedBoxes; }

		// �Փ˔���̕\���ɓo�^����֐�
		void AddDisplayCollision(Imase::DisplayCollision* displayCollision);

	};

	// �Փ˔��萶���N���X
	class CollisionFacory
	{
	public:

		static std::unique_ptr<ModelCollision> CreateCollision(ModelCollision::CollisionType type, DirectX::Model* pModel)
		{
			std::unique_ptr<ModelCollision> collision;

			switch (type)
			{
			case ModelCollision::CollisionType::Sphere:
				collision = std::make_unique<ModelCollisionSphere>(pModel);
				break;
			case ModelCollision::CollisionType::AABB:
				collision = std::make_unique<ModelCollisionBox>(pModel);
				break;
			case ModelCollision::CollisionType::OBB:
				collision = std::make_unique<ModelCollisionOrientedBox>(pModel);
				break;
			default:
				break;
			}

			return collision;
		}
	};
}

