//================================================
// 概　要：プレイヤーカメラのプログラム
// 作成日：2024/6/24
// 作成者：松戸浩希
//================================================

#include "pch.h"
#include "GameCamera.h"

using namespace DirectX;

// コンストラクタ
GameCamera::GameCamera()
	: m_type(Type::Type_Back)
	, m_angle(0.0f)
	, m_pPlayerPos{}
	, m_pPlayerRotate{}
{
}

// 更新関数
void GameCamera::Update(float elapsedTime)
{
	switch (m_type)
	{
	case Type::Type_Back:	// プレイヤーの後ろから追いかけるカメラ
		{
			SimpleMath::Vector3 targetPos =
				*m_pPlayerPos + SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
			SimpleMath::Vector3 eyePos(0.0f, 0.5f, -2.0f);
			eyePos = SimpleMath::Vector3::Transform(eyePos, *m_pPlayerRotate);
			SetPositionTarget(targetPos + eyePos, targetPos);
		}
		break;
	case Type::Type_Around:	// プレイヤーの周りを回るカメラ（タイトルシーンにて使用中）
		{
			m_angle += XMConvertToRadians(30.0f) * elapsedTime;
			SimpleMath::Vector3 targetPos =
				*m_pPlayerPos + SimpleMath::Vector3(0.0f, 0.3f, 0.0f);
			SimpleMath::Vector3 eyePos(0.0f, 1.0f, 3.0f);
			eyePos = SimpleMath::Vector3::Transform(
				eyePos, SimpleMath::Matrix::CreateRotationY(m_angle));
			SetPositionTarget(targetPos + eyePos, targetPos);
		}
		break;
	default:
		break;
	}
}