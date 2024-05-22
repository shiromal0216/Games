#include "pch.h"
#include "StageSelectScene.h"
#include "GameStageScene.h"

using namespace DirectX;

StageSelectScene::StageSelectScene()
{
}

void StageSelectScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	// 閉じているシーンをオープンさせる
	auto transitionMask = GetUserResources()->GetTransitionMask();
	transitionMask->Open();
}

void StageSelectScene::Update(float elapsedTime)
{
	// 次のシーンへ
	auto transitionMask = GetUserResources()->GetTransitionMask();
	auto kbTracker = GetUserResources()->GetKeyboardStateTracker();
	auto kb = kbTracker->GetLastState();

	// ------------------------------------- //
	// 上下キーでフレームを移動
	// ------------------------------------- //
	if (kbTracker->pressed.Up && m_freamPos.y > m_freamMin)
	{
		m_freamPos.y -= 140.0f;
	}
	if (kbTracker->pressed.Down && m_freamPos.y < m_freamMax)
	{
		m_freamPos.y += 140.0f;
	}

	// オープンしていてEnterキーが押された
	if (transitionMask->IsOpen() && kbTracker->pressed.Enter && m_freamPos.y == m_freamMin)
	{
		// クローズする
		transitionMask->Close();
	}
	// クローズしたら別のシーンへ
	if (transitionMask->IsClose() && transitionMask->IsEnd())
	{
		ChangeScene<GameStageScene>();
	}
}

void StageSelectScene::Render()
{
	m_spriteBatch->Begin();

	// 「背景」画像を描画する
	m_spriteBatch->Draw(m_backGroundSRV.Get(), SimpleMath::Vector2(BACKGROUND_POSITION));
	// 「ステージ１」画像を描画する
	m_spriteBatch->Draw(m_stage1ButtonSRV.Get(), SimpleMath::Vector2(STAGE1_POSITION));
	// 「ステージ２」画像を描画する
	m_spriteBatch->Draw(m_stage2ButtonSRV.Get(), SimpleMath::Vector2(STAGE2_POSITION));
	// 「ステージ３」画像を描画する
	m_spriteBatch->Draw(m_stage3ButtonSRV.Get(), SimpleMath::Vector2(STAGE3_POSITION));
	// 「フレーム」画像を描画する
	m_spriteBatch->Draw(m_freamSRV.Get(), SimpleMath::Vector2(FREAM_POSITION) + m_freamPos);
	// 「選択案内」画像を描画する
	m_spriteBatch->Draw(m_selectButtonSRV.Get(), SimpleMath::Vector2(SELECT_POSITION));
	// 「選択決定」画像を描画する
	m_spriteBatch->Draw(m_selectEnterSRV.Get(), SimpleMath::Vector2(ENTER_POSITION));

	if (m_freamPos.y == m_freamMin) {
		// 「ステージ１内容」画像を描画する
		m_spriteBatch->Draw(m_stage1ContSRV.Get(), SimpleMath::Vector2(STAGECONT_POSITION));
	}
	if (m_freamPos.y == m_freamMiddle){
		// 「ステージ２内容」画像を描画する
		m_spriteBatch->Draw(m_stage2ContSRV.Get(), SimpleMath::Vector2(STAGECONT_POSITION));
	}
	if (m_freamPos.y == m_freamMax) {
		// 「ステージ３内容」画像を描画する
		m_spriteBatch->Draw(m_stage3ContSRV.Get(), SimpleMath::Vector2(STAGECONT_POSITION));
	}

	m_spriteBatch->End();
}

void StageSelectScene::Finalize()
{
	m_stage1ButtonSRV.Reset();
	m_stage2ButtonSRV.Reset();
	m_stage3ButtonSRV.Reset();
	m_stage1ContSRV.Reset();
	m_stage2ContSRV.Reset();
	m_stage3ContSRV.Reset();
	m_freamSRV.Reset();
	m_selectButtonSRV.Reset();
	m_selectEnterSRV.Reset();
	m_backGroundSRV.Reset();

	m_spriteBatch.reset();
}

void StageSelectScene::CreateDeviceDependentResources()
{
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();

	// スプライトバッチの作成
	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	// 「ステージ１」画像の読み込み
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/Stage1Button.dds", nullptr, m_stage1ButtonSRV.ReleaseAndGetAddressOf())
	);
	// 「ステージ２」画像の読み込み
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/Stage2Button.dds", nullptr, m_stage2ButtonSRV.ReleaseAndGetAddressOf())
	);
	// 「ステージ３」画像の読み込み
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/Stage3Button.dds", nullptr, m_stage3ButtonSRV.ReleaseAndGetAddressOf())
	);
	// 「ステージ１内容」画像の読み込み
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/Stage1Contents.dds", nullptr, m_stage1ContSRV.ReleaseAndGetAddressOf())
	);
	// 「ステージ２内容」画像の読み込み
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/Stage2Contents.dds", nullptr, m_stage2ContSRV.ReleaseAndGetAddressOf())
	);
	// 「ステージ３内容」画像の読み込み
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/Stage3Contents.dds", nullptr, m_stage3ContSRV.ReleaseAndGetAddressOf())
	);
	// 「フレーム」画像の読み込み
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/SelectFream.dds", nullptr, m_freamSRV.ReleaseAndGetAddressOf())
	);
	// 「選択案内」画像の読み込み
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/SelectButton.dds", nullptr, m_selectButtonSRV.ReleaseAndGetAddressOf())
	);
	// 「選択決定」画像の読み込み
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/SelectEnter.dds", nullptr, m_selectEnterSRV.ReleaseAndGetAddressOf())
	);
	// 「背景」画像の読み込み
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/BackGround.dds", nullptr, m_backGroundSRV.ReleaseAndGetAddressOf())
	);
}

void StageSelectScene::CreateWindowSizeDependentResources()
{
}

void StageSelectScene::OnDeviceLost()
{
	Finalize();
}
