#pragma once

namespace CollisionTest
{
	// �Փ˔���p�I�u�W�F�N�g�̍\����
	struct Object
	{
		// �ʒu
		DirectX::SimpleMath::Vector3 position;

		// ��]
		DirectX::SimpleMath::Quaternion rotate;

		// -------------------------------------------------------- //

		// �Փ˔���i���j
		DirectX::BoundingSphere boundingSphere;

		// �Փ˔�����擾�֐��i���j
		DirectX::BoundingSphere GetBoundingSphere()
		{
			DirectX::BoundingSphere tmp(boundingSphere);
			tmp.Center = DirectX::SimpleMath::Vector3::Transform(boundingSphere.Center, rotate) + position;
			return tmp;
		}

		// -------------------------------------------------------- //

		// �Փ˔���iAABB�j
		DirectX::BoundingBox boundingBox;

		// �Փ˔�����擾�֐��iAABB�j
		DirectX::BoundingBox GetBoundingBox()
		{
			DirectX::BoundingBox tmp(boundingBox);
			tmp.Center = DirectX::SimpleMath::Vector3::Transform(boundingBox.Center, rotate) + position;
			return tmp;
		}

		// -------------------------------------------------------- //

		// �Փ˔�����擾�֐��iOBB�j
		DirectX::BoundingOrientedBox GetBoundingOrientedBox()
		{
			DirectX::BoundingOrientedBox tmp;
			DirectX::BoundingOrientedBox::CreateFromBoundingBox(tmp, boundingBox);
			tmp.Center = DirectX::SimpleMath::Vector3::Transform(boundingBox.Center, rotate) + position;
			tmp.Orientation = rotate;
			return tmp;
		}
	};
}
