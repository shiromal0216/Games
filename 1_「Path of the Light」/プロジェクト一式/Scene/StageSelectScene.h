#pragma once

#include "ImaseLib/SceneManager.h"
#include "ImaseLib/UserResources.h"
#include "ImaseLib/ResourceManager.h"

class StageSelectScene : public Imase::Scene<UserResources>
{
public:
	
	// コンストラクタ
	StageSelectScene();

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

	// スプライトバッチへのポインタ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// ステージ１ボタンのテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stage1ButtonSRV;
	// ステージ２ボタンのテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stage2ButtonSRV;
	// ステージ３ボタンのテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stage3ButtonSRV;
	// ステージ１内容画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stage1ContSRV;
	// ステージ２内容画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stage2ContSRV;
	// ステージ３内容画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stage3ContSRV;
	// フレームのテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_freamSRV;
	// 選択案内画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_selectButtonSRV;
	// 選択決定画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_selectEnterSRV;
	// 背景画像のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backGroundSRV;


private:

	// フレームの位置
	DirectX::SimpleMath::Vector2 m_freamPos;

	// フレームの最小の位置
	float m_freamMin = 0.0f;
	// フレームの中間の位置
	float m_freamMiddle = 140.0f;
	// フレームの最大の位置
	float m_freamMax = 280.0f;

private:

	// ステージ１ボタンのテクスチャハンドルの位置
	DirectX::SimpleMath::Vector2 STAGE1_POSITION = DirectX::SimpleMath::Vector2(150.0f, 175.0f);
	// ステージ２ボタンのテクスチャハンドルの位置
	DirectX::SimpleMath::Vector2 STAGE2_POSITION = DirectX::SimpleMath::Vector2(150.0f, 315.0f);
	// ステージ３ボタンのテクスチャハンドルの位置
	DirectX::SimpleMath::Vector2 STAGE3_POSITION = DirectX::SimpleMath::Vector2(150.0f, 455.0f);
	// ステージ内容のテクスチャハンドルの位置
	DirectX::SimpleMath::Vector2 STAGECONT_POSITION = DirectX::SimpleMath::Vector2(700.0f, 100.0f);
	// フレームのテクスチャハンドルの位置
	DirectX::SimpleMath::Vector2 FREAM_POSITION = DirectX::SimpleMath::Vector2(133.0f, 153.0f);
	// 選択案内画像のテクスチャハンドルの位置
	DirectX::SimpleMath::Vector2 SELECT_POSITION = DirectX::SimpleMath::Vector2(50.0f, 600.0f);
	// 選択決定画像のテクスチャハンドルの位置
	DirectX::SimpleMath::Vector2 ENTER_POSITION = DirectX::SimpleMath::Vector2(420.0f, 600.0f);
	// 背景画像のテクスチャハンドルの位置
	DirectX::SimpleMath::Vector2 BACKGROUND_POSITION = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
};
