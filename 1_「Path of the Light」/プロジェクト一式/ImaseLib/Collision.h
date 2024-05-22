//--------------------------------------------------------------------------------------
// File: Collision.h
//
// �Փ˔���p�R���W�����N���X
// ���l�F�Q�[���v���O���~���O�̂��߂̃��A���^�C���Փ˔�����Q�l�ɍ쐬
//
// Date: 2023.6.15
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

namespace Imase
{
	class Collision
	{
	public:
		//------------------------------//
		// �����ƎO�p�`�̌�������p		//
		//------------------------------//

		// �O�p�`�̍\���́i�����ƎO�p�`�̌�������p�j
		struct Triangle
		{
			// �O�p�`�̕��ʕ�����
			DirectX::SimpleMath::Plane p;
			// ��BC�̕��ʕ������i�d�S���W�̒��_a�ɑ΂���d��u��^����j
			DirectX::SimpleMath::Plane edgePlaneBC;
			// ��CA�̕��ʕ������i�d�S���W�̒��_b�ɑ΂���d��v��^����j
			DirectX::SimpleMath::Plane edgePlaneCA;

			// �R���X�g���N�^���ŏՓ˔�����y�����邽�߂ɑO�����Ōv�Z���Ă���
			Triangle(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b, DirectX::SimpleMath::Vector3 c)
			{
				DirectX::SimpleMath::Vector3 n = (c - a).Cross(b - a);
				p = DirectX::SimpleMath::Plane(a, n);
				DirectX::SimpleMath::Plane pp = DirectX::SimpleMath::Plane(b, n);
				edgePlaneBC = DirectX::SimpleMath::Plane(b, n.Cross(b - c));
				edgePlaneCA = DirectX::SimpleMath::Plane(c, n.Cross(c - a));
				p.Normalize(); edgePlaneBC.Normalize(); edgePlaneCA.Normalize();
				float bc_scale = 1.0f / (a.Dot(edgePlaneBC.Normal()) + edgePlaneBC.D());
				float ca_scale = 1.0f / (b.Dot(edgePlaneCA.Normal()) + edgePlaneCA.D());
				edgePlaneBC.x *= bc_scale; edgePlaneBC.y *= bc_scale; edgePlaneBC.z *= bc_scale; edgePlaneBC.w *= bc_scale;
				edgePlaneCA.x *= ca_scale; edgePlaneCA.y *= ca_scale; edgePlaneCA.z *= ca_scale; edgePlaneCA.w *= ca_scale;
			}
		};

		/// <summary>
		/// �����ƎO�p�`�̌�������
		/// </summary>
		/// <param name="p">�����̎n�_</param>
		/// <param name="q">�����̏I�_</param>
		/// <param name="tri">�O�p�`</param>
		/// <param name="s">�����ƎO�p�`�̌����_</param>
		/// <returns>true�̏ꍇ�������Ă���</returns>
		static bool IntersectSegmentTriangle(DirectX::SimpleMath::Vector3 p, DirectX::SimpleMath::Vector3 q, Triangle tri, DirectX::SimpleMath::Vector3* s)
		{
			// ���������_�̌덷�œ�����ʂ�����̂ŏ����]�T������
			constexpr float EPSILON = 1.0e-06F;
			float distp = p.Dot(tri.p.Normal()) + tri.p.D();
			if (distp < 0.0f) return false;
			float distq = q.Dot(tri.p.Normal()) + tri.p.D();
			if (distq >= 0.0f) return false;
			float denom = distp - distq;
			float t = distp / denom;
			*s = p + t * (q - p);
			float u = s->Dot(tri.edgePlaneBC.Normal()) + tri.edgePlaneBC.D();
			if (fabsf(u) < EPSILON) u = 0.0f;
			if (u < 0.0f || u > 1.0f) return false;
			float v = s->Dot(tri.edgePlaneCA.Normal()) + tri.edgePlaneCA.D();
			if (fabsf(v) < EPSILON) v = 0.0f;
			if (v < 0.0f) return false;
			float w = 1.0f - u - v;
			if (fabsf(w) < EPSILON) w = 0.0f;
			if (w < 0.0f) return false;
			return true;
		}

	};
}


