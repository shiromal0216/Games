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

class GameStageScene : public Imase::Scene<UserResources>
{
public:
	
	// コンストラクタ
	GameStageScene();

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

	// 衝突判定の登録関数
	//void CollisionRegist();

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

	// ロボットの上半身の回転
	DirectX::SimpleMath::Quaternion m_bodyRotate;

	// ロボットの左腕の回転
	DirectX::SimpleMath::Quaternion m_armLRotate;

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
	std::unique_ptr <DirectX::Model> m_lightModel;
	std::unique_ptr <DirectX::Model> m_missileModel;

	enum { ROOT, HEAD, BODY, LEG, ARM_R, ARM_L, LIGHT, MISSILE, PARTS_CNT };

	// ロボットのパーツへのポインタ
	std::unique_ptr<Imase::ModelPart> m_parts[PARTS_CNT];

	// ミサイル発射フラグ
	bool m_fireFlag;

	// ミサイルの位置
	DirectX::SimpleMath::Vector3 m_missilePosition;

	// ミサイルの回転
	DirectX::SimpleMath::Quaternion m_missileRotate;

	// ミサイルの移動距離
	float m_distance;

	// 光の玉取得フラグ
	bool m_lightBallFlag;

	// 「何もない」画像表示フラグ
	bool m_nothingFlag;

	// 「光の玉獲得」画像表示フラグ
	bool m_lightgetFlag;

	// 「鍵獲得」画像表示フラグ
	bool m_keygetFlag;

	// 「鍵がかかっている」画像表示フラグ
	bool m_lockFlag;

	// 「鍵」画像表示フラグ
	bool m_keyFlag;

	// 「バール」画像表示フラグ
	bool m_crowbarFlag;

	// 「バール獲得」画像表示フラグ
	bool m_bargetFlag;

	// 「何も起きない」画像表示フラグ
	bool m_notgetFlag;

	// 「引っ掛かっていて開かない」画像表示フラグ
	bool m_stuckFlag;

	// 「バールでこじ開けた」画像表示フラグ
	bool m_barOpenFlag;

	// 「鍵で開けた」画像表示フラグ
	bool m_keyOpenFlag;

	// 「地図」画像表示フラグ
	bool m_mapFlag;

	// 「ゲーム案内」画像表示フラグ
	bool m_guideFlag;

	// 「ステージクリア」画像表示フラグ
	bool m_stageClearFlag;

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
	// 宝箱モデルへのポインタ
	std::unique_ptr<DirectX::Model> m_treasureModel;
	// WEC木箱モデルへのポインタ
	std::unique_ptr<DirectX::Model> m_WECBoxModel;
	// 木箱モデルへのポインタ
	std::unique_ptr<DirectX::Model> m_woodBoxModel;
	// 戸棚モデルへのポインタ
	std::unique_ptr<DirectX::Model> m_cabinetModel;
	// 鉢植えモデルへのポインタ
	std::unique_ptr<DirectX::Model> m_pottedModel;
	// 壁モデルへのポインタ
	std::unique_ptr<DirectX::Model> m_wallModel;
	// ベッドモデルへのポインタ
	std::unique_ptr<DirectX::Model> m_bedModel;
	// ランタンモデルへのポインタ
	std::unique_ptr<DirectX::Model> m_lanternModel;
	// 机モデルへのポインタ
	std::unique_ptr<DirectX::Model> m_deskModel;

private:

	// 「ロボットの移動」の説明画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_robotMoveSRV;
	// 「ロボットの上半身の回転」の説明画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_upbodySRV;
	// 「左腕(ライト)の回転」の説明画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ArmLSRV;
	// 「E調べる」画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_eCheckSRV;
	// 「光の玉」画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_lightBallSRV;
	// 「何もない」画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_nothingSRV;
	// 「光の玉獲得」画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_lightgetSRV;
	// 「鍵獲得」画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_keygetSRV;
	// 「鍵」画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_keySRV;
	// 「鍵がかかっている」画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_lockSRV;
	// 「Eはめる」画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_eSetSRV;
	// 「ステージクリア」画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stageClearSRV;
	// 「何も起きない」画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_notgetSRV;
	// 「ゲームの案内」画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_guideSRV;
	// 「アイテムフレーム」画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_itemFrameSRV;
	// 「バール」画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_crowBarSRV;
	// 「バール獲得」画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_bargetSRV;
	// 「引っ掛かって開かない」画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stuckSRV;
	// 「バールでこじ開けた」画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_barOpenSRV;
	// 「鍵で開けた」画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_keyOpenSRV;
	// 「地図」画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_mapSRV;

private:

	// シャドウマップのサイズ
	static const int SHADOWMAP_SIZE = 512;

	// ------------------------------------- //
	// モデルの位置の定数化
	// ------------------------------------- //
	// 宝箱の位置
	const DirectX::SimpleMath::Vector3 TREASURE_POSITION = { 3.0f, 0.0f, 4.0f };
	// WEC木箱の位置
	const DirectX::SimpleMath::Vector3 WECBOX_POSITION = { 3.0f, 0.0f, 2.0f };
	// 木箱の位置
	const DirectX::SimpleMath::Vector3 WOODBOX_POSITION = { 3.0f, 0.0f, 0.0f };
	// 戸棚の位置
	const DirectX::SimpleMath::Vector3 CABINET_POSITION = { 3.0f, 0.0f, -3.0f };
	// 鉢植えの位置
	const DirectX::SimpleMath::Vector3 POTTED_POSITION = { -3.0f, 0.0f, 3.0f };
	// ベッドの位置
	const DirectX::SimpleMath::Vector3 BED_POSITION = { -3.0f, 0.0f, -3.0f };
	// ランタンの位置
	const DirectX::SimpleMath::Vector3 LANTERN_POSITION = { -3.0f,0.0f,0.0f };
	// 机の位置
	const DirectX::SimpleMath::Vector3 DESK_POSITION = { 0.0f,0.0f,2.0f };

	// ------------------------------------- //
	// 画像の位置の定数化
	// ------------------------------------- //
	// 「ロボットの移動」画像の位置
	const DirectX::SimpleMath::Vector2 MOVE_TEXTURE_POSITION = { 1135.0f, 450.0f };
	// 「ロボットの上半身の回転」画像の位置
	const DirectX::SimpleMath::Vector2 BODY_TEXTURE_POSITION = { 1145.0f, 550.0f };
	// 「左腕(ライト)の回転」画像の位置
	const DirectX::SimpleMath::Vector2 RIGHT_TEXTURE_POSITION = { 1120.0f, 640.0f };
	// 「E調べる」「E入る」画像の位置
	const DirectX::SimpleMath::Vector2 E_TEXTURE_POSITION = { 200.0f,200.0f };
	// 「光の玉」画像の位置
	const DirectX::SimpleMath::Vector2 LIGHTBALL_POSITION = { 120.0f,615.0f };
	// 「鍵」画像の位置
	const DirectX::SimpleMath::Vector2 KEY_POSITION = { 105.0f,592.0f };
	// 「ステージクリア」画像の位置
	const DirectX::SimpleMath::Vector2 STAGECLEAR_POSITION = { 50.0f, 100.0f };
	// 「スタート案内」画像の位置
	const DirectX::SimpleMath::Vector2 GUIDE_POSITION = { 100.0f,200.0f };
	// 「何も起きないようだ」画像の位置
	const DirectX::SimpleMath::Vector2 NOTHING_POSITION = { 100.0f,200.0f };
	// 「アイテムフレーム」画像の位置
	const DirectX::SimpleMath::Vector2 ITEMFRAME_POSITION = { 5.0f,605.0f };
	// 「バール」画像の位置
	const DirectX::SimpleMath::Vector2 CROWBAR_POSITION = { 10.0f,612.0f };
	// 「地図」画像の位置
	const DirectX::SimpleMath::Vector2 MAP_POSITION = { 150.0f,25.0f };

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

	// シャドウマップ用（レンダーテクスチャ）
	std::unique_ptr<DX::RenderTexture> m_shadowMapRT;

	// シャドウマップ用（デプスステンシル）
	std::unique_ptr<Imase::DepthStencil> m_shadowMapDS;

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

	// OBB判定関数
	bool HitCheckOBB(
		const DirectX::BoundingOrientedBox& box1,
		const DirectX::BoundingOrientedBox& box2
	);

	// 衝突判定用の表示オブジェクトへのポインタ
	//std::unique_ptr<Imase::DisplayCollision> m_displayCollision;

	// 衝突判定用オブジェクト
	CollisionTest::Object m_robotColl[5];	// ロボット
	CollisionTest::Object m_modelColl[8];	// 各モデル(宝箱等)
	CollisionTest::Object m_wallColl[4];	// 壁
};

