//================================================
// 概　要：プレイヤーカメラのプログラム
// 作成日：2024/6/24
// 作成者：松戸浩希
//================================================

#pragma once

#include "ImaseLib/Camera.h"

// ゲームカメラ
class GameCamera : public Matsudo::Camera
{
public:

	// カメラのタイプ
	enum class Type
	{
		Type_Back,	// プレイヤーの後ろから追いかけるカメラ
		Type_Around	// プレイヤーの周りを回るカメラ
	};

private:

	// 現在のカメラのタイプ
	Type m_type;

	// 回転角（ラジアン）
	float m_angle;

	// プレイヤーの位置
	const DirectX::SimpleMath::Vector3* m_pPlayerPos;

	// プレイヤーの回転
	const DirectX::SimpleMath::Quaternion* m_pPlayerRotate;

public:

	// コンストラクタ
	GameCamera();

	// 更新関数
	void Update(float elapsedTime);

	// プレイヤーの位置と回転を設定する関数
	void SetPlayer(
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Quaternion& rotate
	)
	{
		m_pPlayerPos = &position;
		m_pPlayerRotate = &rotate;
	}

	// カメラのタイプの切り替え関数
	void SetType(Type type) { m_type = type; }

};
