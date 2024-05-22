#include "pch.h"
#include "TitleScene.h"
#include "DebugDraw.h"
#include "ReadData.h"
#include <iomanip>
#include "StageSelectScene.h"
#include "GameStageScene.h"

using namespace DirectX;

TitleScene::TitleScene()
{
	// スポットライトの範囲の角度
	m_lightTheta = 45.0f;

	// ライトの位置
	m_lightPosition = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
}

void TitleScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();

	// デバッグカメラの作成
	RECT rect = GetUserResources()->GetDeviceResources()->GetOutputSize();
	m_debugCamera = std::make_unique<Imase::DebugCamera>(rect.right, rect.bottom);

	// カメラにプレイヤーに位置と回転を渡す
	m_camera.SetPlayer(m_robotPosition, m_robotRotate);

	// ロボットの各パーツの作成
	m_parts[ROOT] = std::make_unique<Imase::ModelPart>();
	m_parts[HEAD] = std::make_unique<Imase::ModelPart>(m_headModel.get());
	m_parts[BODY] = std::make_unique<Imase::ModelPart>(m_bodyModel.get());
	m_parts[LEG]  = std::make_unique<Imase::ModelPart>(m_legModel.get());
	m_parts[ARM_R] = std::make_unique<Imase::ModelPart>(m_armRModel.get());
	m_parts[ARM_L] = std::make_unique<Imase::ModelPart>(m_armLModel.get());
	m_parts[RIGHT] = std::make_unique<Imase::ModelPart>(m_rightModel.get());
	m_parts[MISSILE] = std::make_unique<Imase::ModelPart>(m_missileModel.get());

	// ロボットの各パーツを連結する
	m_parts[ROOT]->SetChild(m_parts[LEG].get());
	m_parts[LEG]->SetChild(m_parts[BODY].get());
	m_parts[BODY]->SetChild(m_parts[HEAD].get());
	m_parts[HEAD]->SetSibling(m_parts[ARM_R].get());
	m_parts[ARM_R]->SetSibling(m_parts[ARM_L].get());
	m_parts[ARM_R]->SetChild(m_parts[MISSILE].get());
	m_parts[ARM_L]->SetChild(m_parts[RIGHT].get());

	// 各パーツの初期位置行列を作成する
	m_parts[BODY]->SetInitialMatrix(
		SimpleMath::Matrix::CreateTranslation(0.0f, 0.32f, 0.0f));
	m_parts[HEAD]->SetInitialMatrix(
		SimpleMath::Matrix::CreateTranslation(0.0f, 0.75f, 0.0f));
	m_parts[ARM_R]->SetInitialMatrix(
		SimpleMath::Matrix::CreateTranslation(-0.18f, 0.72f, 0.0f));
	//左腕に位置情報
	m_leftArmPosition = SimpleMath::Vector3(0.18f, 0.72f, 0.0f);
	m_parts[ARM_L]->SetInitialMatrix(
		SimpleMath::Matrix::CreateTranslation(m_leftArmPosition));
	m_parts[RIGHT]->SetInitialMatrix(
		SimpleMath::Matrix::CreateTranslation(0.28f, 0.52f, 0.39f));
	m_parts[MISSILE]->SetInitialMatrix(
		SimpleMath::Matrix::CreateTranslation(-0.28f, 0.52f, 0.39f));

	// 連結されているパーツの初期位置行列を親の座標系に全て変換する
	m_parts[ROOT]->SetupMatrix();

	// 定数バッファの内容更新
	ConstantBuffer2 cb = {};
	cb.fCosTheta = cosf(XMConvertToRadians(m_lightTheta / 2.0f));
	context->UpdateSubresource(m_constantBuffer2.Get(), 0, nullptr, &cb, 0, 0);
}

void TitleScene::Update(float elapsedTime)
{
	elapsedTime;

	// 次のシーンへ
	auto transitionMask = GetUserResources()->GetTransitionMask();
	auto kbTracker = GetUserResources()->GetKeyboardStateTracker();
	auto kb = kbTracker->GetLastState();

	// デバッグカメラの更新
	m_debugCamera->Update();

	// カメラ回転
	m_camera.SetType(GameCamera::Type::Type_B);

	// ロボットの移動
	m_robotPosition += SimpleMath::Vector3::Transform(SimpleMath::Vector3(0.0f, 0.0f, 0.01f), m_robotRotate);

	//ライトをロボットの腕に移動させる
	m_lightPosition = m_robotPosition + m_leftArmPosition;

	// カメラの更新
	m_camera.Update(elapsedTime);

	// -------------------------------------------- //
	// シーン切り替え
	// -------------------------------------------- //
	// オープンしていてEnterキーが押された
	if (transitionMask->IsOpen() && kbTracker->pressed.Enter)
	{
		// クローズする
		transitionMask->Close();
	}
	// クローズしたら別のシーンへ
	if (transitionMask->IsClose() && transitionMask->IsEnd())
	{
		ChangeScene <StageSelectScene>();
	}
}

void TitleScene::Render()
{
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = GetUserResources()->GetCommonStates();

	// -------------------------------------------------------------------------- //
	// Pass1 シャドウマップの作成
	// -------------------------------------------------------------------------- //

	auto rtv = m_shadowMapRT->GetRenderTargetView();
	auto srv = m_shadowMapRT->GetShaderResourceView();
	auto dsv = m_shadowMapDS->GetDepthStencilView();

	// レンダーターゲットを変更（shadowMapRT）
	context->ClearRenderTargetView(rtv, SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f));
	context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
	context->OMSetRenderTargets(1, &rtv, dsv);

	// ビューポートを設定
	D3D11_VIEWPORT vp = { 0.0f, 0.0f, SHADOWMAP_SIZE, SHADOWMAP_SIZE, 0.0f, 1.0f };
	context->RSSetViewports(1, &vp);

	m_view = SimpleMath::Matrix::CreateLookAt(
		m_camera.GetEyePosition(),
		m_camera.GetTargetPosition(),
		SimpleMath::Vector3::UnitY
	);

	// ------------------------------------------------ //
	// ライト空間のビュー行列と射影行列を作成する
	// ------------------------------------------------ //

	// ライトの方向
	SimpleMath::Vector3 lightDir = SimpleMath::Vector3::Transform(SimpleMath::Vector3(0.0f, 0.0f, 1.0f), m_lightRotate);

	// ビュー行列を作成
	SimpleMath::Matrix view = SimpleMath::Matrix::CreateLookAt(
		m_lightPosition,
		m_lightPosition + lightDir,
		SimpleMath::Vector3::UnitY
	);

	// 射影行列を作成
	SimpleMath::Matrix proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(m_lightTheta), 1.0f, 0.1f, 100.0f);

	// -------------------------------------------------------------------------- //
	// 定数バッファを更新
	// -------------------------------------------------------------------------- //

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	// GPUが定数バッファに対してアクセスを行わないようにする
	context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// 定数バッファを更新
	ConstantBuffer cb = {};
	SimpleMath::Matrix m = view * proj;
	cb.lightViewProj = XMMatrixTranspose(m);
	cb.lightPosition = m_lightPosition;
	cb.lightDirection = lightDir;
	cb.lightAmbient = SimpleMath::Color(0.2f, 0.2f, 0.2f);	// ステージの明るさ調整

	*static_cast<ConstantBuffer*>(mappedResource.pData) = cb;

	// GPUが定数バッファに対してのアクセスを許可する
	context->Unmap(m_constantBuffer.Get(), 0);

	// -------------------------------------------------------------------------- //
	// レンダーターゲットとビューポートを元に戻す
	// -------------------------------------------------------------------------- //
	auto renderTarget = GetUserResources()->GetDeviceResources()->GetRenderTargetView();
	auto depthStencil = GetUserResources()->GetDeviceResources()->GetDepthStencilView();

	context->ClearRenderTargetView(renderTarget, Colors::Black);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);
	auto const viewport = GetUserResources()->GetDeviceResources()->GetScreenViewport();
	context->RSSetViewports(1, &viewport);
	
	SimpleMath::Matrix world;								// ↓床の大きさ
	world = SimpleMath::Matrix::CreateScale(SimpleMath::Vector3(10.0f,1.0f,10.0f))
		* SimpleMath::Matrix::CreateTranslation(0.0f, 0.0f, 0.0f);

	// 床の描画
	m_floorModel->Draw(context, *states, world, m_view, m_proj, false, [&]()
		{
			// 定数バッファの設定
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// 作成したシャドウマップをリソースとして設定
			context->PSSetShaderResources(1, 1, &srv);

			// テクスチャサンプラーの設定
			ID3D11SamplerState* samplers[] = { states->PointWrap(), m_shadowMapSampler.Get()};
			context->PSSetSamplers(0, 2, samplers);

			// シェーダーの設定
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	// リソースの割り当てを解除する（shadowMapRT）
	ID3D11ShaderResourceView* nullsrv[] = { nullptr };
	context->PSSetShaderResources(1, 1, nullsrv);

	// ロボットを移動させる
	SimpleMath::Matrix move = SimpleMath::Matrix::CreateFromQuaternion(m_robotRotate)
		* SimpleMath::Matrix::CreateTranslation(m_robotPosition);
	m_parts[ROOT]->SetTransformMatrix(move);

	// 影の描画関数
	DrawShadow(context, states, m_robotPosition, 1.0f);

	// ロボットの描画
	m_parts[ROOT]->UpdateMatrix();
	m_parts[ROOT]->Draw(context, *states, m_view, m_proj, [&]()
		{
			// カリングしない
			context->RSSetState(states->CullNone());
		}
	);

	m_spriteBatch->Begin();

	// タイトルロゴ画像の描画
	m_spriteBatch->Draw(m_titleSRV.Get(), SimpleMath::Vector2(TITLE_POSITION));

	// Press to start画像の描画
	m_spriteBatch->Draw(m_startSRV.Get(), SimpleMath::Vector2(START_POSITION));

	m_spriteBatch->End();
}

void TitleScene::Finalize()
{
	m_headModel.reset();
	m_bodyModel.reset();
	m_legModel.reset();
	m_armRModel.reset();
	m_armLModel.reset();
	m_rightModel.reset();
	m_missileModel.reset();

	m_floorModel.reset();
	m_spriteBatch.reset();

	m_titleSRV.Reset();
	m_startSRV.Reset();

	// 影の終了処理
	ResetShadow();
}

void TitleScene::CreateDeviceDependentResources()
{
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();

	// エフェクトファクトリーの作成
	EffectFactory fx(device);
	fx.SetDirectory(L"Resources/Models");

	std::unique_ptr<EffectFactory> fxrbt = std::make_unique<EffectFactory>(device);
	fxrbt->SetDirectory(L"Resources/Models");

	// ロボットの各パーツモデルの作成
	m_headModel = Model::CreateFromCMO(device, L"Resources/Models/Head.cmo", *fxrbt);
	m_bodyModel = Model::CreateFromCMO(device, L"Resources/Models/Body.cmo", *fxrbt);
	m_legModel = Model::CreateFromCMO(device, L"Resources/Models/Leg.cmo", *fxrbt);
	m_armRModel = Model::CreateFromCMO(device, L"Resources/Models/Arm_R.cmo", *fxrbt);
	m_armLModel = Model::CreateFromCMO(device, L"Resources/Models/Arm_L.cmo", *fxrbt);
	m_rightModel = Model::CreateFromCMO(device, L"Resources/Models/SpotLight.cmo", *fxrbt);
	m_missileModel = Model::CreateFromCMO(device, L"Resources/Models/Missile.cmo", *fxrbt);

	// 影の初期化関数
	InitializeShadow(device, context);

	// 床のモデルの作成
	m_floorModel = Model::CreateFromCMO(device, L"Resources/Models/Floor.cmo", fx);

	// スポットライトのモデルの作成
	m_spotLightModel = Model::CreateFromCMO(device, L"Resources/Models/SpotLight.cmo", fx);

	// 自己発光するようにエフェクトを設定する
	m_spotLightModel->UpdateEffects([&](IEffect* effect)
		{
			auto basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				basicEffect->SetEmissiveColor(Colors::White);
			}
		}
	);

	RECT rect = { 0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE };

	// レンダーテクスチャの作成（シャドウマップ用）
	m_shadowMapRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R32_FLOAT);
	m_shadowMapRT->SetDevice(device);
	m_shadowMapRT->SetWindow(rect);

	// デプスステンシルの作成（シャドウマップ用）
	m_shadowMapDS = std::make_unique<Imase::DepthStencil>(DXGI_FORMAT_D32_FLOAT);
	m_shadowMapDS->SetDevice(device);
	m_shadowMapDS->SetWindow(rect);

	// スプライトバッチの作成
	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	// タイトルロゴ画像の読み込み
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/Title.dds", nullptr, 
			m_titleSRV.ReleaseAndGetAddressOf())
	);

	// Press to start画像の読み込み
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/Press to start.dds", nullptr,
			m_startSRV.ReleaseAndGetAddressOf())
	);

	// 頂点シェーダーの作成（シャドウマップ用）
	std::vector<uint8_t> vs_depth = DX::ReadData(L"Resources/Shaders/SM_VS_Depth.cso");
	DX::ThrowIfFailed(
		device->CreateVertexShader(vs_depth.data(), vs_depth.size(), nullptr, m_VS_Depth.ReleaseAndGetAddressOf())
	);

	// ピクセルシェーダーの作成（シャドウマップ用）
	std::vector<uint8_t> ps_depth = DX::ReadData(L"Resources/Shaders/SM_PS_Depth.cso");
	DX::ThrowIfFailed(
		device->CreatePixelShader(ps_depth.data(), ps_depth.size(), nullptr, m_PS_Depth.ReleaseAndGetAddressOf())
	);

	// 定数バッファの作成
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = static_cast<UINT>(sizeof(ConstantBuffer));	// 確保するサイズ（16の倍数で設定する）
	// GPU (読み取り専用) と CPU (書き込み専用) の両方からアクセスできるリソース
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// 定数バッファとして扱う
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPUが内容を変更できるようにする
	DX::ThrowIfFailed(device->CreateBuffer(&bufferDesc, nullptr, m_constantBuffer.ReleaseAndGetAddressOf()));

	// 定数バッファの作成
	bufferDesc.ByteWidth = static_cast<UINT>(sizeof(ConstantBuffer2));	// 確保するサイズ（16の倍数で設定する）
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;	// GPUの読み取りと書き込みが可能な一般的なリソース
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// 定数バッファとして扱う
	bufferDesc.CPUAccessFlags = 0;	// CPUはアクセスしないので0
	DX::ThrowIfFailed(device->CreateBuffer(&bufferDesc, nullptr, m_constantBuffer2.ReleaseAndGetAddressOf()));

	// 頂点シェーダーの作成（シャドウマップ用）
	std::vector<uint8_t> vs = DX::ReadData(L"Resources/Shaders/SM_VS.cso");
	DX::ThrowIfFailed(
		device->CreateVertexShader(vs.data(), vs.size(), nullptr, m_VS.ReleaseAndGetAddressOf())
	);

	// ピクセルシェーダーの作成（シャドウマップ用）
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/SM_PS.cso");
	DX::ThrowIfFailed(
		device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_PS.ReleaseAndGetAddressOf())
	);

	// サンプラーの作成（シャドウマップ用）
	D3D11_SAMPLER_DESC sampler_desc = CD3D11_SAMPLER_DESC(D3D11_DEFAULT);
	sampler_desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_LESS;
	device->CreateSamplerState(&sampler_desc, m_shadowMapSampler.ReleaseAndGetAddressOf());

}

void TitleScene::CreateWindowSizeDependentResources()
{
	// 射影行列を作成
	RECT rect = GetUserResources()->GetDeviceResources()->GetOutputSize();
	m_proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 200.0f
	);
}

void TitleScene::OnDeviceLost()
{
	Finalize();
}

// 影の初期化関数
void TitleScene::InitializeShadow(ID3D11Device* device, ID3D11DeviceContext* context)
{
	// ベーシックエフェクトの作成
	m_basicEffect = std::make_unique<BasicEffect>(device);
	// ライティング(OFF)
	m_basicEffect->SetLightingEnabled(false);
	// 頂点カラー(OFF)
	m_basicEffect->SetVertexColorEnabled(false);
	// テクスチャ(ON)
	m_basicEffect->SetTextureEnabled(true);

	// 入力レイアウトの作成
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionTexture>(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf()
		)
	);

	// プリミティブバッチの作成
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	// 影のテクスチャの読み込み
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(
			device,
			L"Resources/Textures/Shadow.dds",
			nullptr,
			m_shadowTexture.ReleaseAndGetAddressOf()
		)
	);
}

// 影の終了処理
void TitleScene::ResetShadow()
{
	m_basicEffect.reset();
	m_primitiveBatch.reset();
}

// 影の描画関数
void TitleScene::DrawShadow(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	DirectX::SimpleMath::Vector3 position,
	float radius
)
{
	// エフェクトの設定＆適応
	m_basicEffect->SetWorld(SimpleMath::Matrix::Identity);
	m_basicEffect->SetView(m_view);
	m_basicEffect->SetProjection(m_proj);
	m_basicEffect->SetTexture(m_shadowTexture.Get());
	m_basicEffect->Apply(context);

	// 入力レイアウトの設定
	context->IASetInputLayout(m_inputLayout.Get());

	// テクスチャサンプラの設定
	ID3D11SamplerState* sampler[] = { states->LinearClamp() };
	context->PSSetSamplers(0, 1, sampler);

	// アルファブレンドの設定
	context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xffffffff);

	// 深度ステンシルの設定
	context->OMSetDepthStencilState(m_depthStencilState_Shadow.Get(), 1);

	// 影の頂点情報
	VertexPositionTexture vertexes[] =
	{
		VertexPositionTexture(SimpleMath::Vector3::Zero, SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3::Zero, SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3::Zero, SimpleMath::Vector2(0.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3::Zero, SimpleMath::Vector2(1.0f, 1.0f)),
	};
	// 影のインデックス情報
	uint16_t indexes[] = { 0, 1, 2, 1, 3, 2 };

	// 影の表示位置の設定
	vertexes[0].position = SimpleMath::Vector3(-radius, 0.01f, -radius) + position;
	vertexes[1].position = SimpleMath::Vector3(radius, 0.01f, -radius) + position;
	vertexes[2].position = SimpleMath::Vector3(-radius, 0.01f, radius) + position;
	vertexes[3].position = SimpleMath::Vector3(radius, 0.01f, radius) + position;

	// 影の描画
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawIndexed(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		indexes, _countof(indexes),
		vertexes, _countof(vertexes)
	);
	m_primitiveBatch->End();
}
