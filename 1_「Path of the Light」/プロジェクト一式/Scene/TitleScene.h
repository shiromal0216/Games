//================================================
// 概　要：タイトルシーンのプログラム
// 作成日：2024/4/22
// 作成者：マツド コウキ
//================================================

#pragma once

#include "ImaseLib/SceneManager.h"
#include "ImaseLib/UserResources.h"
#include "ImaseLib/DebugCamera.h"
#include "ImaseLib/DisplayCollision.h"
#include "Object.h"
#include "ImaseLib/ModelCollision.h"
#include "GameCamera.h"
#include "ImaseLib/ModelPart.h"
#include "RenderTexture.h"
#include "ImaseLib/DepthStencil.h"

class TitleScene : public Imase::Scene<UserResources>
{
public:
	
	// コンストラクタ
	TitleScene();

	// 初期化
	void Initialize() override;

	// 更新
	void Update(float elapsedTime) override;

	// 描画
	void Render() override;

	// 終了処理
	void Finalize() override;

	// デバイスに依存するリソースを作成する関数
	void CreateDeviceDependentResources() override;

	// ウインドウサイズに依存するリソースを作成する関数
	void CreateWindowSizeDependentResources() override;

	// デバイスロストした時に呼び出される関数
	void OnDeviceLost() override;

private:

	// デバッグカメラへのポインタ
	std::unique_ptr<Imase::DebugCamera> m_debugCamera;

	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;

	// 射影行列
	DirectX::SimpleMath::Matrix m_proj;

private:
	// ロボットの位置
	DirectX::SimpleMath::Vector3 m_robotPosition;

	// ロボットの回転
	DirectX::SimpleMath::Quaternion m_robotRotate;

	// ロボットの左腕の位置
	DirectX::SimpleMath::Vector3 m_leftArmPosition;

	// カメラ
	GameCamera m_camera;

	// ロボットの各パーツモデルへのポインタ
	std::unique_ptr <DirectX::Model> m_headModel;
	std::unique_ptr <DirectX::Model> m_bodyModel;
	std::unique_ptr <DirectX::Model> m_legModel;
	std::unique_ptr <DirectX::Model> m_armRModel;
	std::unique_ptr <DirectX::Model> m_armLModel;
	std::unique_ptr <DirectX::Model> m_rightModel;
	std::unique_ptr <DirectX::Model> m_missileModel;

	enum { ROOT, HEAD, BODY, LEG, ARM_R, ARM_L, RIGHT, MISSILE, PARTS_CNT };

	// ロボットのパーツへのポインタ
	std::unique_ptr<Matsudo::ModelPart> m_parts[PARTS_CNT];

private:

	// 床モデルへのポインタ
	std::unique_ptr<DirectX::Model> m_floorModel;

	// 影のテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shadowTexture;

	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatch;

	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// 影の初期化関数
	void InitializeShadow(ID3D11Device* device, ID3D11DeviceContext* context);

	// 影の終了処理
	void ResetShadow();

	// 影の描画関数
	void DrawShadow(
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		DirectX::SimpleMath::Vector3 position,
		float radius = 0.5f
	);

	// 深度ステンシルステート（床用）
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState_Floor;

	// 深度ステンシルステート（影用）
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState_Shadow;

private:

	// レンダーテクスチャ(シーン全体)
	std::unique_ptr<DX::RenderTexture> m_offscreenRT;

	// レンダーテクスチャ(ブルーム用)
	std::unique_ptr<DX::RenderTexture> m_blur1RT;
	std::unique_ptr<DX::RenderTexture> m_blur2RT;

	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// ベーシックポストプロセス
	std::unique_ptr<DirectX::BasicPostProcess> m_basicPostProcess;

	// デュアルポストプロセス
	std::unique_ptr<DirectX::DualPostProcess> m_dualPostProcess;

private:
	
	// シャドウマップのサイズ
	static const int SHADOWMAP_SIZE = 512;

	// シャドウマップ用（レンダーテクスチャ）
	std::unique_ptr<DX::RenderTexture> m_shadowMapRT;

	// シャドウマップ用（デプスステンシル）
	std::unique_ptr<Matsudo::DepthStencil> m_shadowMapDS;

	// 頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS_Depth;

	// ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS_Depth;

	// ライトの位置
	DirectX::SimpleMath::Vector3 m_lightPosition;

	// ライトの回転
	DirectX::SimpleMath::Quaternion m_lightRotate;

private:

	// 定数バッファの構造体
	struct ConstantBuffer
	{
		DirectX::XMMATRIX lightViewProj;	// ライトの投影空間へ座標変換する行列
		DirectX::XMVECTOR lightPosition;	// ライトの位置
		DirectX::XMVECTOR lightDirection;	// ライトの方向
		DirectX::XMVECTOR lightAmbient;		// ライトの環境光
	};

	// 定数バッファへのポインタ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

	// 定数バッファの構造体（頻繁に更新されない物）
	struct ConstantBuffer2
	{
		float fCosTheta;		// スポットライトのfov/2
		float pad[3];
	};

	// 定数バッファへのポインタ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer2;

	// 頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS;

	// ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS;

	// サンプラー
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_shadowMapSampler;

	// スポットライトのモデルへのポインタ
	std::unique_ptr<DirectX::Model> m_spotLightModel;

	// スポットライトの範囲の角度
	float m_lightTheta;

private:

	// タイトルロゴ画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_titleSRV;

	// Press to start画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_startSRV;

	// ------------------------------------------------------- //
	// 画像の位置の定数化
	// ------------------------------------------------------- //
	// タイトルロゴ画像の位置
	const DirectX::SimpleMath::Vector2 TITLE_POSITION = { 60.0f, 50.0f };
	// Press to start画像のテクスチャハンドル
	const DirectX::SimpleMath::Vector2 START_POSITION = { 400.0f, 500.0f };

};

