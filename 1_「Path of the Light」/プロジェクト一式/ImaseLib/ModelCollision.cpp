//--------------------------------------------------------------------------------------
// File: Collision.cpp
//
// ���f���p�Փ˔���N���X
//
// Date: 2023.6.10
// Author: Hideyasu Imase
//
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "ModelCollision.h"

using namespace DirectX;
using namespace Imase;

// �R���X�g���N�^
ModelCollision::ModelCollision(CollisionType type)
	: m_type(type)
{
}

// ------------------------------------------------------------------------------- //

// �R���X�g���N�^
ModelCollisionSphere::ModelCollisionSphere(DirectX::Model* pModel)
	: ModelCollision(ModelCollision::CollisionType::Sphere)
{
	// �R���W�����������f���f�[�^����擾
	for (auto& mesh : pModel->meshes) {
		m_srcSpheres.push_back(&mesh->boundingSphere);
	}
	// ���[���h��ԓ��̃R���W��������ۑ����郏�[�N���m�ۂ���
	m_dstSpheres.resize(pModel->meshes.size());
}

// �Փ˔�����̍X�V
void ModelCollisionSphere::UpdateBoundingInfo(
	DirectX::SimpleMath::Vector3 position,
	DirectX::SimpleMath::Quaternion rotate
)
{
	UNREFERENCED_PARAMETER(rotate);

	for (int i = 0; i < m_srcSpheres.size(); i++)
	{
		DirectX::BoundingSphere dst(*m_srcSpheres[i]);
		dst.Center = DirectX::SimpleMath::Vector3::Transform(m_srcSpheres[i]->Center, rotate) + position;
		m_dstSpheres[i] = dst;
	}
}

// �Փ˔�����s���֐�
bool ModelCollisionSphere::Intersects(ModelCollision* collision)
{
	return ModelCollision::Intersects<ModelCollisionSphere, DirectX::BoundingSphere>(this, collision);
}

// �������s���֐�
bool ModelCollisionSphere::Contains(ModelCollision* collision)
{
	return ModelCollision::Contains<ModelCollisionSphere, DirectX::BoundingSphere>(this, collision);
}

// �Փ˔���̕\���ɓo�^����֐�
void ModelCollisionSphere::AddDisplayCollision(Imase::DisplayCollision* displayCollision)
{
	for (auto& data : GetBoundingInfo())
	{
		displayCollision->AddBoundingSphere(data);
	}
}

// ------------------------------------------------------------------------------- //

// �R���X�g���N�^
ModelCollisionBox::ModelCollisionBox(DirectX::Model* pModel)
	: ModelCollision(ModelCollision::CollisionType::AABB)
{
	// �R���W�����������f���f�[�^����擾
	for (auto& mesh : pModel->meshes) {
		m_srcBoxes.push_back(&mesh->boundingBox);
	}
	// ���[���h��ԓ��̃R���W��������ۑ����郏�[�N���m�ۂ���
	m_dstBoxes.resize(pModel->meshes.size());
}

// �Փ˔�����̍X�V
void ModelCollisionBox::UpdateBoundingInfo(
	DirectX::SimpleMath::Vector3 position,
	DirectX::SimpleMath::Quaternion rotate
)
{
	UNREFERENCED_PARAMETER(rotate);

	for (int i = 0; i < m_srcBoxes.size(); i++)
	{
		DirectX::BoundingBox dst(*m_srcBoxes[i]);
		dst.Center = DirectX::SimpleMath::Vector3::Transform(m_srcBoxes[i]->Center, rotate) + position;
		m_dstBoxes[i] = dst;
	}
}

// �Փ˔�����s���֐�
bool ModelCollisionBox::Intersects(ModelCollision* collision)
{
	return ModelCollision::Intersects<ModelCollisionBox, DirectX::BoundingBox>(this, collision);
}

// �������s���֐�
bool ModelCollisionBox::Contains(ModelCollision* collision)
{
	return ModelCollision::Contains<ModelCollisionBox, DirectX::BoundingBox>(this, collision);
}

// �Փ˔���̕\���ɓo�^����֐�
void ModelCollisionBox::AddDisplayCollision(Imase::DisplayCollision* displayCollision)
{
	for (auto& data : GetBoundingInfo())
	{
		displayCollision->AddBoundingBox(data);
	}
}

// ------------------------------------------------------------------------------- //

// �R���X�g���N�^
ModelCollisionOrientedBox::ModelCollisionOrientedBox(DirectX::Model* pModel)
	: ModelCollision(ModelCollision::CollisionType::OBB)
{
	// �R���W�����������f���f�[�^����擾
	for (auto& mesh : pModel->meshes) {
		m_srcBoxes.push_back(&mesh->boundingBox);
	}
	// ���[���h��ԓ��̃R���W��������ۑ����郏�[�N���m�ۂ���
	m_dstOrientedBoxes.resize(pModel->meshes.size());
}

// �Փ˔�����̍X�V
void ModelCollisionOrientedBox::UpdateBoundingInfo(
	DirectX::SimpleMath::Vector3 position,
	DirectX::SimpleMath::Quaternion rotate
)
{
	for (int i = 0; i < m_srcBoxes.size(); i++)
	{
		DirectX::BoundingOrientedBox dst;
		DirectX::BoundingOrientedBox::CreateFromBoundingBox(dst, *m_srcBoxes[i]);
		dst.Center = DirectX::SimpleMath::Vector3::Transform(m_srcBoxes[i]->Center, rotate) + position;
		dst.Orientation = rotate;
		m_dstOrientedBoxes[i] = dst;
	}
}

// �Փ˔�����s���֐�
bool ModelCollisionOrientedBox::Intersects(ModelCollision* collision)
{
	return ModelCollision::Intersects<ModelCollisionOrientedBox, DirectX::BoundingOrientedBox>(this, collision);
}

// �������s���֐�
bool ModelCollisionOrientedBox::Contains(ModelCollision* collision)
{
	return ModelCollision::Contains<ModelCollisionOrientedBox, DirectX::BoundingOrientedBox>(this, collision);
}

// �Փ˔���̕\���ɓo�^����֐�
void ModelCollisionOrientedBox::AddDisplayCollision(Imase::DisplayCollision* displayCollision)
{
	for (auto& data : GetBoundingInfo())
	{
		displayCollision->AddBoundingOrientedBox(data);
	}
}
