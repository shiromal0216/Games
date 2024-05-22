#include "pch.h"
#include "GameStageScene.h"
#include "DebugDraw.h"
#include "ReadData.h"
#include "StageSelectScene.h"

using namespace DirectX;

#define WALL_NUM 4		// 壁の数
#define OBJECT_NUM 7	// モデルの数

GameStageScene::GameStageScene()
	: m_lightRotate{}	  , m_lightPosition{}
	, m_lightTheta(45.0f) , m_distance(0.0f)
	, m_fireFlag(false)   , m_lightBallFlag(false)
	, m_nothingFlag(false), m_lightgetFlag(false)
	, m_keygetFlag(false) , m_keyFlag(false)
	, m_lockFlag(false)	  , m_guideFlag(true)
	, m_notgetFlag(false) , m_stageClearFlag(false)
	, m_crowbarFlag(false), m_bargetFlag(false)
	, m_stuckFlag(false)  , m_barOpenFlag(false)
	, m_keyOpenFlag(false), m_mapFlag(false)
{
}

void GameStageScene::Initialize()
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
	m_parts[LIGHT] = std::make_unique<Imase::ModelPart>(m_lightModel.get());
	m_parts[MISSILE] = std::make_unique<Imase::ModelPart>(m_missileModel.get());

	// ロボットの各パーツを連結する
	m_parts[ROOT]->SetChild(m_parts[LEG].get());
	m_parts[LEG]->SetChild(m_parts[BODY].get());
	m_parts[BODY]->SetChild(m_parts[HEAD].get());
	m_parts[HEAD]->SetSibling(m_parts[ARM_R].get());
	m_parts[ARM_R]->SetSibling(m_parts[ARM_L].get());
	m_parts[ARM_R]->SetChild(m_parts[MISSILE].get());
	m_parts[ARM_L]->SetChild(m_parts[LIGHT].get());
	// 各パーツの初期位置行列を作成する
	m_parts[BODY]->SetInitialMatrix(
		SimpleMath::Matrix::CreateTranslation(0.0f, 0.32f, 0.0f));
	m_parts[HEAD]->SetInitialMatrix(
		SimpleMath::Matrix::CreateTranslation(0.0f, 0.75f, 0.0f));
	m_parts[ARM_R]->SetInitialMatrix(
		SimpleMath::Matrix::CreateTranslation(-0.18f, 0.72f, 0.0f));
	// 左腕に位置情報
	m_leftArmPosition = SimpleMath::Vector3(0.18f, 0.72f, 0.0f);
	m_parts[ARM_L]->SetInitialMatrix(
		SimpleMath::Matrix::CreateTranslation(m_leftArmPosition));
	m_parts[LIGHT]->SetInitialMatrix(
		SimpleMath::Matrix::CreateTranslation(0.28f, 0.52f, 0.39f));
	m_parts[MISSILE]->SetInitialMatrix(
		SimpleMath::Matrix::CreateTranslation(-0.28f, 0.52f, 0.39f));

	// 連結されているパーツの初期位置行列を親の座標系に全て変換する
	m_parts[ROOT]->SetupMatrix();

	// 定数バッファの内容更新
	ConstantBuffer2 cb = {};
	cb.fCosTheta = cosf(XMConvertToRadians(m_lightTheta / 2.0f));
	context->UpdateSubresource(m_constantBuffer2.Get(), 0, nullptr, &cb, 0, 0);

	// 閉じているシーンをオープンさせる
	auto transitionMask = GetUserResources()->GetTransitionMask();
	transitionMask->Open();

	// ---------------------------------------------- //
	// ロボットモデルのコリジョン設定
	// ---------------------------------------------- //
	// ロボット（体）
	m_robotColl[0].boundingBox = m_bodyModel->meshes[0]->boundingBox;
	// ロボット（脚）
	m_robotColl[1].boundingBox = m_legModel->meshes[0]->boundingBox;
	// ロボット（右腕）
	m_robotColl[2].boundingBox = m_armRModel->meshes[0]->boundingBox;
	// ロボット（左腕）
	m_robotColl[3].boundingBox = m_armLModel->meshes[0]->boundingBox;
	// ロボット（ミサイル）
	m_robotColl[4].boundingBox = m_missileModel->meshes[0]->boundingBox;

	// ---------------------------------------------- //
	// 各モデルのコリジョン設定
	// ---------------------------------------------- //
	// 宝箱
	m_modelColl[0].boundingBox = m_treasureModel->meshes[0]->boundingBox;
	// WEC木箱
	m_modelColl[1].boundingBox = m_treasureModel->meshes[0]->boundingBox;
	// 木箱
	m_modelColl[2].boundingBox = m_woodBoxModel->meshes[0]->boundingBox;
	// 戸棚
	m_modelColl[3].boundingBox = m_cabinetModel->meshes[0]->boundingBox;
	// 鉢植え
	m_modelColl[4].boundingBox = m_pottedModel->meshes[0]->boundingBox;
	// ベッド
	m_modelColl[5].boundingBox = m_bedModel->meshes[0]->boundingBox;
	// 机
	m_modelColl[6].boundingBox = m_deskModel->meshes[0]->boundingBox;
	// ランタン
	m_modelColl[7].boundingBox = m_lanternModel->meshes[0]->boundingBox;

	// 宝箱の衝突判定の座標設定
	m_modelColl[0].position = SimpleMath::Vector3(TREASURE_POSITION);
	// WEC木箱の衝突判定の座標設定
	m_modelColl[1].position = SimpleMath::Vector3(WECBOX_POSITION);
	// 木箱の衝突判定の座標設定
	m_modelColl[2].position = SimpleMath::Vector3(WOODBOX_POSITION);
	// 戸棚の衝突判定の座標設定
	m_modelColl[3].position = SimpleMath::Vector3(CABINET_POSITION);
	// 鉢植えの衝突判定の座標設定
	m_modelColl[4].position = SimpleMath::Vector3(POTTED_POSITION);
	// ベッドの衝突判定の座標設定
	m_modelColl[5].position = SimpleMath::Vector3(BED_POSITION);
	// 机の衝突判定の座標設定
	m_modelColl[6].position = SimpleMath::Vector3(DESK_POSITION);
	// ランタンの衝突判定の座標設定
	m_modelColl[7].position = SimpleMath::Vector3(LANTERN_POSITION);

	// ---------------------------------------------- //
	// 壁のコリジョン設定
	// ---------------------------------------------- //
	m_wallColl[0].boundingBox = m_wallModel->meshes[0]->boundingBox;	// 左壁の大きさ
	m_wallColl[1].boundingBox = m_wallModel->meshes[0]->boundingBox;	// 右壁の大きさ
	m_wallColl[2].boundingBox = m_wallModel->meshes[0]->boundingBox;	// 真正面の壁の大きさ
	m_wallColl[3].boundingBox = m_wallModel->meshes[0]->boundingBox;	// 真後ろの壁の大きさ

	m_wallColl[0].position = SimpleMath::Vector3(5.0f, 0.0f, 0.0f);		// 左壁の位置
	m_wallColl[1].position = SimpleMath::Vector3(-5.0f, 0.0f, 0.0f);	// 右壁の位置
	m_wallColl[2].position = SimpleMath::Vector3(0.0f, 0.0f, 5.0f);		// 真正面の壁の位置
	m_wallColl[3].position = SimpleMath::Vector3(0.0f, 0.0f, -5.0f);	// 真後ろの壁の位置

	m_wallColl[0].rotate = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, XMConvertToRadians(90.0f));
	m_wallColl[1].rotate = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, XMConvertToRadians(90.0f));
}

void GameStageScene::Update(float elapsedTime)
{
	elapsedTime;

	auto transitionMask = GetUserResources()->GetTransitionMask();
	auto kbTracker = GetUserResources()->GetKeyboardStateTracker();
	auto kb = kbTracker->GetLastState();

	// デバッグカメラの更新
	m_debugCamera->Update();

	//=====ロボットの移動=====//
	// 各案内の画像が出ている場合は処理しない
	if (!m_bargetFlag  && !m_barOpenFlag && !m_lockFlag     &&
		!m_guideFlag   && !m_notgetFlag  && !m_nothingFlag  &&
		!m_bargetFlag  && !m_stuckFlag   && !m_barOpenFlag  &&
		!m_keyOpenFlag && !m_mapFlag	 && !m_lightgetFlag && !m_keygetFlag)
	{
		// 前進
		if (kb.W)
		{
			m_robotPosition += SimpleMath::Vector3::Transform(SimpleMath::Vector3(0.0f, 0.0f, 0.05f), m_robotRotate);
		}
		// 後進
		if (kb.S)
		{
			m_robotPosition += SimpleMath::Vector3::Transform(SimpleMath::Vector3(0.0f, 0.0f, -0.05f), m_robotRotate);
		}
		// 左旋回
		if (kb.A)
		{
			m_robotRotate *= SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, XMConvertToRadians(1.5f));
		}
		// 右旋回
		if (kb.D)
		{
			m_robotRotate *= SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, XMConvertToRadians(-1.5f));
		}
	}

	//=====ロボットの上半身の回転=====//
	if (kb.Left)
	{
		m_bodyRotate *= SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, XMConvertToRadians(1.5f));
	}
	if (kb.Right)
	{
		m_bodyRotate *= SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, XMConvertToRadians(-1.5f));
	}

	//=====ロボットの左腕の回転=====//
	if (kb.Up)
	{
		m_armLRotate *= SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitX, XMConvertToRadians(-1.5f));
	}
	if (kb.Down)
	{
		m_armLRotate *= SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitX, XMConvertToRadians(1.5f));
	}

	//ロボット回転 *  体回転 * 腕の回転
	m_lightRotate = m_robotRotate * m_bodyRotate * m_armLRotate;

	//ライトをロボットの腕に移動させる
	m_lightPosition = m_robotPosition + m_leftArmPosition;

	// スペースキーでミサイルを発射
	if (!m_fireFlag && kbTracker->pressed.Space)
	{
		m_fireFlag = true;
		// ミサイルを左腕から外す
		m_parts[ARM_R]->SetChild(nullptr);
		// ミサイルのワールド行列を取得
		SimpleMath::Matrix m = m_parts[MISSILE]->GetWorldMatrix();
		// ミサイルの位置を取得
		m_missilePosition.x = m._41;
		m_missilePosition.y = m._42;
		m_missilePosition.z = m._43;
		// ミサイルの回転を取得
		m_missileRotate = SimpleMath::Quaternion::CreateFromRotationMatrix(m);
	}

	// ミサイル発射中なら
	if (m_fireFlag)
	{
		// ミサイルを回転させる
		m_missileRotate = SimpleMath::Quaternion::CreateFromAxisAngle(
			SimpleMath::Vector3::UnitZ, XMConvertToRadians(5.0f)) * m_missileRotate;
		// ミサイルを向いている方向へ移動させる
		SimpleMath::Vector3 v(0.0f, 0.0f, 0.15f);
		m_missilePosition += SimpleMath::Vector3::Transform(v, m_missileRotate);
		m_distance += v.Length();
		// 移動距離が5ｍを超えたら腕にミサイルを戻す
		if (m_distance > 5.0f)
		{
			m_fireFlag = false;
			m_distance = 0.0f;
			m_parts[ARM_R]->SetChild(m_parts[MISSILE].get());
		}
	}

	// ---------------------------------- //
	// シーンの切り替え
	// ---------------------------------- //
	// ステージクリア後にタイトルシーンへ戻る
	if (m_stageClearFlag  && kbTracker->pressed.Enter)
	{
		// クローズする
		transitionMask->Close();
	}
	// クローズしたら別のシーンへ
	if (m_stageClearFlag  && transitionMask->IsClose() && transitionMask->IsEnd())
	{
		ChangeScene<StageSelectScene>();
	}
	
	// ロボットの衝突判定の座標設定
	m_robotColl[0].position = m_robotPosition + SimpleMath::Vector3(0.0f,0.32f,0.0f);		// ロボット（体）
	m_robotColl[1].position = m_robotPosition;												// ロボット（脚）
	m_robotColl[2].position = m_robotPosition + SimpleMath::Vector3(-0.18f,0.72f,0.0f);		// ロボット（右腕）
	m_robotColl[3].position = m_robotPosition + SimpleMath::Vector3(0.18f, 0.72f, 0.0f);	// ロボット（左腕）
	m_robotColl[4].position = m_missilePosition;											// ロボット（ミサイル）

	m_robotColl[0].rotate = m_robotRotate;
	m_robotColl[1].rotate = m_robotRotate;
	m_robotColl[2].rotate = m_robotRotate * m_bodyRotate;
	m_robotColl[3].rotate = m_robotRotate * m_bodyRotate;
	m_robotColl[4].rotate = m_robotRotate;

	// -------------------------------------------------- //
	// 壁の当たり判定の押し戻し処理
	// -------------------------------------------------- //
	for (int i = 0; i < WALL_NUM; i++)
	{
		if (HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_wallColl[i].GetBoundingOrientedBox()))
		{
			m_robotPosition -= SimpleMath::Vector3::Transform(SimpleMath::Vector3(0.0f, 0.0f, 0.05f), m_robotRotate);
		}
	}

	// カメラの更新
	m_camera.Update(elapsedTime);
}

void GameStageScene::Render()
{
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = GetUserResources()->GetCommonStates();

	// -------------------------------------------------------------------------- //
	// シャドウマップの作成
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
	cb.lightAmbient = SimpleMath::Color(0.15f, 0.15f, 0.15f);	// ステージの明るさ調整

	*static_cast<ConstantBuffer*>(mappedResource.pData) = cb;

	// GPUが定数バッファに対してのアクセスを許可する
	context->Unmap(m_constantBuffer.Get(), 0);

	// ------------------------------------------------ //
	// 影になるモデルを描画する
	// ------------------------------------------------ //

	SimpleMath::Matrix shadow;

	// 宝箱の影の描画
	shadow = SimpleMath::Matrix::CreateTranslation(TREASURE_POSITION);	// 宝箱の影の位置
	m_treasureModel->Draw(context, *states, shadow, view, proj, false, [&]()
		{
			context->VSSetShader(m_VS_Depth.Get(), nullptr, 0);
			context->PSSetShader(m_PS_Depth.Get(), nullptr, 0);
		}
	);

	// WEC木箱の影の描画
	shadow = SimpleMath::Matrix::CreateTranslation(WECBOX_POSITION);	// WEC木箱の影の位置
	m_WECBoxModel->Draw(context, *states, shadow, view, proj, false, [&]()
		{
			context->VSSetShader(m_VS_Depth.Get(), nullptr, 0);
			context->PSSetShader(m_PS_Depth.Get(), nullptr, 0);
		}
	);

	// 木箱の影の描画
	shadow = SimpleMath::Matrix::CreateTranslation(WOODBOX_POSITION);	// 木箱の影の位置
	m_woodBoxModel->Draw(context, *states, shadow, view, proj, false, [&]()
		{
			context->VSSetShader(m_VS_Depth.Get(), nullptr, 0);
			context->PSSetShader(m_PS_Depth.Get(), nullptr, 0);
		}
	);

	// 戸棚の影の描画
	shadow = SimpleMath::Matrix::CreateTranslation(CABINET_POSITION);	// 戸棚の影の位置
	m_cabinetModel->Draw(context, *states, shadow, view, proj, false, [&]()
		{
			context->VSSetShader(m_VS_Depth.Get(), nullptr, 0);
			context->PSSetShader(m_PS_Depth.Get(), nullptr, 0);
		}
	);

	// 鉢植えの影の描画
	shadow = SimpleMath::Matrix::CreateTranslation(POTTED_POSITION);	// 鉢植えの影の位置
	m_pottedModel->Draw(context, *states, shadow, view, proj, false, [&]()
		{
			context->VSSetShader(m_VS_Depth.Get(), nullptr, 0);
			context->PSSetShader(m_PS_Depth.Get(), nullptr, 0);
		}
	);

	// ベッドの影の描画
	shadow = SimpleMath::Matrix::CreateTranslation(BED_POSITION);	// ベッドの影の位置
	m_bedModel->Draw(context, *states, shadow, view, proj, false, [&]()
		{
			context->VSSetShader(m_VS_Depth.Get(), nullptr, 0);
			context->PSSetShader(m_PS_Depth.Get(), nullptr, 0);
		}
	);

	// ランタンの影の描画
	shadow = SimpleMath::Matrix::CreateTranslation(LANTERN_POSITION);	// ランタンの影の位置
	m_lanternModel->Draw(context, *states, shadow, view, proj, false, [&]()
		{
			context->VSSetShader(m_VS_Depth.Get(), nullptr, 0);
			context->PSSetShader(m_PS_Depth.Get(), nullptr, 0);
		}
	);

	// 机の影の描画
	shadow = SimpleMath::Matrix::CreateTranslation(DESK_POSITION);	// 机の影の位置
	m_deskModel->Draw(context, *states, shadow, view, proj, false, [&]()
		{
			context->VSSetShader(m_VS_Depth.Get(), nullptr, 0);
			context->PSSetShader(m_PS_Depth.Get(), nullptr, 0);
		}
	);

	// -------------------------------------------------------------------------- //
	// レンダーターゲットとビューポートを元に戻す
	// -------------------------------------------------------------------------- //
	auto renderTarget = GetUserResources()->GetDeviceResources()->GetRenderTargetView();
	auto depthStencil = GetUserResources()->GetDeviceResources()->GetDepthStencilView();
	
	auto kbTracker = GetUserResources()->GetKeyboardStateTracker();
	auto kb = kbTracker->GetLastState();

	context->ClearRenderTargetView(renderTarget, Colors::Black);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);
	auto const viewport = GetUserResources()->GetDeviceResources()->GetScreenViewport();
	context->RSSetViewports(1, &viewport);


	// ---------------------------------------------------------------- //
	// 各モデルの描画
	// ---------------------------------------------------------------- //
	SimpleMath::Matrix model;
															// ↓床の大きさ変更
	model = SimpleMath::Matrix::CreateScale(SimpleMath::Vector3(1.0f,1.0f,1.0f)) * SimpleMath::Matrix::CreateTranslation(0.0f, 0.0f, 0.0f);

	// 床の描画
	m_floorModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
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

	// --------------------------------------------------------------------------------- //
	// 壁モデルの描画
	// --------------------------------------------------------------------------------- // 
												// ↓壁の位置変更
	model = SimpleMath::Matrix::CreateTranslation(0.0f, 0.0f, 5.0f);
	// 壁の描画（真正面）
	m_wallModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
		{
			// 定数バッファの設定
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// 作成したシャドウマップをリソースとして設定
			context->PSSetShaderResources(1, 1, &srv);

			// テクスチャサンプラーの設定
			ID3D11SamplerState* samplers[] = { states->PointWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// シェーダーの設定
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	model = SimpleMath::Matrix::CreateTranslation(0.0f, 0.0f, 5.0f)
		* SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(90.0f));
	// 壁の描画（左壁）
	m_wallModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
		{
			// 定数バッファの設定
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// 作成したシャドウマップをリソースとして設定
			context->PSSetShaderResources(1, 1, &srv);

			// テクスチャサンプラーの設定
			ID3D11SamplerState* samplers[] = { states->PointWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// シェーダーの設定
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	model = SimpleMath::Matrix::CreateTranslation(0.0f, 0.0f, -5.0f)
		* SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(90.0f));
	// 壁の描画（右壁）
	m_wallModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
		{
			// 定数バッファの設定
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// 作成したシャドウマップをリソースとして設定
			context->PSSetShaderResources(1, 1, &srv);

			// テクスチャサンプラーの設定
			ID3D11SamplerState* samplers[] = { states->PointWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// シェーダーの設定
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	model = SimpleMath::Matrix::CreateTranslation(0.0f, 0.0f, -5.0f);
	// 壁の描画（真後ろ）
	m_wallModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
		{
			// 定数バッファの設定
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// 作成したシャドウマップをリソースとして設定
			context->PSSetShaderResources(1, 1, &srv);

			// テクスチャサンプラーの設定
			ID3D11SamplerState* samplers[] = { states->PointWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// シェーダーの設定
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	// ---------------------------------------------------------------------------- //
	// 各モデルの描画
	// ---------------------------------------------------------------------------- //
	// 宝箱の描画
	model = SimpleMath::Matrix::CreateTranslation(TREASURE_POSITION);	// 宝箱の位置
	m_treasureModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
		{
			// 定数バッファの設定
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// 作成したシャドウマップをリソースとして設定
			context->PSSetShaderResources(1, 1, &srv);

			// テクスチャサンプラーの設定
			ID3D11SamplerState* samplers[] = { states->LinearWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// シェーダーの設定
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	// WEC木箱の描画
	model = SimpleMath::Matrix::CreateTranslation(WECBOX_POSITION);	// WEC木箱の位置
	m_WECBoxModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
		{
			// 定数バッファの設定
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// 作成したシャドウマップをリソースとして設定
			context->PSSetShaderResources(1, 1, &srv);

			// テクスチャサンプラーの設定
			ID3D11SamplerState* samplers[] = { states->LinearWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// シェーダーの設定
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	// 木箱の描画
	model = SimpleMath::Matrix::CreateTranslation(WOODBOX_POSITION); // 木箱の位置
	m_woodBoxModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
		{
			// 定数バッファの設定
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// 作成したシャドウマップをリソースとして設定
			context->PSSetShaderResources(1, 1, &srv);

			// テクスチャサンプラーの設定
			ID3D11SamplerState* samplers[] = { states->LinearWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// シェーダーの設定
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	// 戸棚の描画
	model = SimpleMath::Matrix::CreateTranslation(CABINET_POSITION); // 戸棚の位置
	m_cabinetModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
		{
			// 定数バッファの設定
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// 作成したシャドウマップをリソースとして設定
			context->PSSetShaderResources(1, 1, &srv);

			// テクスチャサンプラーの設定
			ID3D11SamplerState* samplers[] = { states->LinearWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// シェーダーの設定
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	// 鉢植えの描画
	model = SimpleMath::Matrix::CreateTranslation(POTTED_POSITION); // 鉢植えの位置
	m_pottedModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
		{
			// 定数バッファの設定
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// 作成したシャドウマップをリソースとして設定
			context->PSSetShaderResources(1, 1, &srv);

			// テクスチャサンプラーの設定
			ID3D11SamplerState* samplers[] = { states->LinearWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// シェーダーの設定
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	// ベッドの描画
	model = SimpleMath::Matrix::CreateTranslation(BED_POSITION); // ベッドの位置
	m_bedModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
		{
			// 定数バッファの設定
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// 作成したシャドウマップをリソースとして設定
			context->PSSetShaderResources(1, 1, &srv);

			// テクスチャサンプラーの設定
			ID3D11SamplerState* samplers[] = { states->LinearWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// シェーダーの設定
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	// ランタンの描画
	model = SimpleMath::Matrix::CreateTranslation(LANTERN_POSITION);	// ランタンの位置
	m_lanternModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
		{
			// 定数バッファの設定
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// 作成したシャドウマップをリソースとして設定
			context->PSSetShaderResources(1, 1, &srv);

			// テクスチャサンプラーの設定
			ID3D11SamplerState* samplers[] = { states->LinearWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// シェーダーの設定
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	// 机の描画
	model = SimpleMath::Matrix::CreateTranslation(DESK_POSITION);	// ランタンの位置
	m_deskModel->Draw(context, *states, model, m_view, m_proj, false, [&]()
		{
			// 定数バッファの設定
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get(), m_constantBuffer2.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 2, cbuf);

			// 作成したシャドウマップをリソースとして設定
			context->PSSetShaderResources(1, 1, &srv);

			// テクスチャサンプラーの設定
			ID3D11SamplerState* samplers[] = { states->LinearWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// シェーダーの設定
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	// リソースの割り当てを解除する（shadowMapRT）
	ID3D11ShaderResourceView* nullsrv[] = { nullptr };
	context->PSSetShaderResources(1, 1, nullsrv);

	SimpleMath::Matrix world;
	world = SimpleMath::Matrix::CreateFromQuaternion(m_lightRotate) * SimpleMath::Matrix::CreateTranslation(m_lightPosition);
	
	// ロボットの上半身の回転
	m_parts[BODY]->SetTransformMatrix(SimpleMath::Matrix::CreateFromQuaternion(m_bodyRotate));

	// ロボットの左腕の回転
	m_parts[ARM_L]->SetTransformMatrix(SimpleMath::Matrix::CreateFromQuaternion(m_armLRotate));

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

	// ミサイル発射中なら
	if (m_fireFlag)
	{
		// ミサイルを描画する
		SimpleMath::Matrix missile = SimpleMath::Matrix::CreateFromQuaternion(m_missileRotate)
			* SimpleMath::Matrix::CreateTranslation(m_missilePosition);
		m_parts[MISSILE]->GetModel()->Draw(context, *states, missile, m_view, m_proj);
	}


	// ----------------------------------------- //
	// 画像の描画ここから
	// ----------------------------------------- //
	m_spriteBatch->Begin();

	// 「ロボットの移動」画像の描画
	m_spriteBatch->Draw(m_robotMoveSRV.Get(), SimpleMath::Vector2(MOVE_TEXTURE_POSITION));

	// 「ロボットの上半身の回転」画像の描画
	m_spriteBatch->Draw(m_upbodySRV.Get(), SimpleMath::Vector2(BODY_TEXTURE_POSITION));

	// 「左腕(ライト)の回転」画像の描画
	m_spriteBatch->Draw(m_ArmLSRV.Get(), SimpleMath::Vector2(RIGHT_TEXTURE_POSITION));

	// 「スタートフレーム」画像の描画
	m_spriteBatch->Draw(m_itemFrameSRV.Get(), SimpleMath::Vector2(ITEMFRAME_POSITION));

	// 「スタート案内」画像の描画
	if (m_guideFlag )
	{
		m_spriteBatch->Draw(m_guideSRV.Get(), SimpleMath::Vector2(GUIDE_POSITION));
	}
	if (m_guideFlag  && kb.Enter)
	{
		// 画像の削除
		m_guideFlag = false;
	}

	// ロボットとランタンが当たったら
	if (HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[7].GetBoundingOrientedBox()))
	{
		// 「Eはめる」画像の描画
		m_spriteBatch->Draw(m_eSetSRV.Get(), SimpleMath::Vector2(E_TEXTURE_POSITION));
	}

	// ロボットと各モデルが当たったら
	for (int i = 0; i < OBJECT_NUM; i++)
	{
		if (HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[i].GetBoundingOrientedBox()))
		{
			// 「E調べる」画像の描画
			m_spriteBatch->Draw(m_eCheckSRV.Get(), SimpleMath::Vector2(E_TEXTURE_POSITION));
		}
	}

	// ---------------------------------- //
	// 「何もないようだ」画像の描画
	// ---------------------------------- //
	// 当たり判定
	if (HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[2].GetBoundingOrientedBox()) && kb.E ||
		HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[4].GetBoundingOrientedBox()) && kb.E ||
		HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[5].GetBoundingOrientedBox()) && kb.E)
	{
		m_nothingFlag = true;
	}
	// 「何もないようだ」画像描画
	if (m_nothingFlag )
	{
		m_spriteBatch->Draw(m_nothingSRV.Get(), SimpleMath::Vector2(GUIDE_POSITION));
	}
	// 「何もないようだ」画像の削除
	if (m_nothingFlag  && kb.Enter)
	{
		m_nothingFlag = false;
	}

	// ---------------------------------- //
	// 「何も起きない」画像の描画
	// ---------------------------------- //
	if (HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[7].GetBoundingOrientedBox())
		&& !m_notgetFlag && !m_lightBallFlag && kb.E)
	{
		m_notgetFlag = true;
	}
	if (m_notgetFlag  && !m_stageClearFlag)
	{
		m_spriteBatch->Draw(m_notgetSRV.Get(), SimpleMath::Vector2(NOTHING_POSITION));
	}
	if (m_notgetFlag  && kb.Enter)
	{
		m_notgetFlag = false;	// 画像の削除
	}

	// ------------------------------------------ //
	// 「引っ掛かっていて開かない」画像の描画
	// ------------------------------------------ //
	if (!m_crowbarFlag &&
		HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[3].GetBoundingOrientedBox()) && kb.E)
	{
		m_stuckFlag = true;
	}
	if (m_stuckFlag )
	{
		m_spriteBatch->Draw(m_stuckSRV.Get(), SimpleMath::Vector2(GUIDE_POSITION));
	}
	// 「引っ掛かっていて開かない」画像の削除
	if (m_stuckFlag  && kb.Enter)
	{
		m_stuckFlag = false;
	}

	// -------------------------------- //
	// 既に光の玉をを持っているなら
	// -------------------------------- //
	if (m_lightBallFlag  && 
		HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[0].GetBoundingOrientedBox()) && kb.E)
	{
		m_nothingFlag = true;
	}
	// 「何もないようだ」画像の描画
	if (m_nothingFlag)
	{
		m_spriteBatch->Draw(m_nothingSRV.Get(), SimpleMath::Vector2(GUIDE_POSITION));
	}

	// ---------------------------------------- //
	// 既に鍵を持っているのなら
	// ---------------------------------------- //
	if (m_keyFlag  && HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[3].GetBoundingOrientedBox()) && kb.E
		|| m_lightBallFlag  && HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[3].GetBoundingOrientedBox()) && kb.E)
	{
		m_nothingFlag = true;
	}
	// 「何もないようだ」画像の描画
	if (m_nothingFlag)
	{
		m_spriteBatch->Draw(m_nothingSRV.Get(), SimpleMath::Vector2(GUIDE_POSITION));
	}

	// -------------------------------- //
	// 既にバールをを持っているなら
	// -------------------------------- //
	if (m_crowbarFlag  &&
		HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[1].GetBoundingOrientedBox()) && kb.E)
	{
		m_nothingFlag = true;
	}
	// 「何もないようだ」画像の描画
	if (m_nothingFlag)
	{
		m_spriteBatch->Draw(m_nothingSRV.Get(), SimpleMath::Vector2(GUIDE_POSITION));
	}

	// ------------------------------------------- //
	// 戸棚と接触中にEキーを押したらバールを取得
	// ------------------------------------------- //
	// 当たり判定
	if (!m_crowbarFlag &&	// バールを持っていない時
		HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[1].GetBoundingOrientedBox()) && kb.E)
	{
		m_bargetFlag = true;
		m_crowbarFlag = true;
	}
	// 「バール獲得」画像の描画
	if (m_bargetFlag)
	{
		m_spriteBatch->Draw(m_bargetSRV.Get(), SimpleMath::Vector2(GUIDE_POSITION));
	}
	// 「バール獲得」画像の削除
	if (m_bargetFlag && kb.Enter)
	{
		m_bargetFlag = false;
	}
	// 「バール」画像の描画
	if (m_crowbarFlag)
	{
		m_spriteBatch->Draw(m_crowBarSRV.Get(), SimpleMath::Vector2(CROWBAR_POSITION));
	}

	// -------------------------------------------- //
	// 戸棚と接触中にEキーを押したら「鍵」を取得
	// -------------------------------------------- //
	// 当たり判定
	if (m_crowbarFlag  && !m_keyFlag && !m_lightBallFlag &&	// バールを持っている、鍵と光の玉を持っていない時
		HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[3].GetBoundingOrientedBox()) && kb.E)
	{
		m_barOpenFlag = true;
	}
	// 「バールをこじ開けた」画像の描画
	if (m_barOpenFlag)
	{
		m_spriteBatch->Draw(m_barOpenSRV.Get(), SimpleMath::Vector2(GUIDE_POSITION));
	}
	// 「鍵獲得」画像の描画
	if (m_keygetFlag)
	{
		m_spriteBatch->Draw(m_keygetSRV.Get(), SimpleMath::Vector2(GUIDE_POSITION));
	}
	// 「鍵」画像の描画
	if (m_keyFlag)
	{
		m_spriteBatch->Draw(m_keySRV.Get(), SimpleMath::Vector2(KEY_POSITION));
	}

	// ---------------------------------------------------------- //
	// 鍵もボールも持っていない時に宝箱に接触し、Eキーを押したら
	// ---------------------------------------------------------- //
	// 当たり判定
	if (!m_keyFlag && !m_lightBallFlag
		&& HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[0].GetBoundingOrientedBox()) && kb.E)
	{
		m_lockFlag = true;
	}
	// 「鍵がかかっている」画像の表示
	if (m_lockFlag)
	{
		m_spriteBatch->Draw(m_lockSRV.Get(), SimpleMath::Vector2(GUIDE_POSITION));
	}
	// 「鍵がかかっている」画像の削除
	if (m_lockFlag && kb.Enter)
	{
		m_lockFlag = false;
	}
	// ------------------------------------ //
	// 鍵を持っていたら光の玉を取得する
	// ------------------------------------ //
	if (m_keyFlag && 
		HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[0].GetBoundingOrientedBox()) && kb.E)
	{
		m_keyOpenFlag = true;
		m_keyFlag = false;	// 鍵画像の削除
	}
	// 「鍵で開けた」画像の描画
	if (m_keyOpenFlag)
	{
		m_spriteBatch->Draw(m_keyOpenSRV.Get(), SimpleMath::Vector2(GUIDE_POSITION));
	}
	// 「光の玉獲得」画像の描画
	if (m_lightgetFlag)
	{
		m_spriteBatch->Draw(m_lightgetSRV.Get(), SimpleMath::Vector2(GUIDE_POSITION));
	}
	// 「光の玉」画像の描画
	if (m_lightBallFlag)
	{
		m_spriteBatch->Draw(m_lightBallSRV.Get(), SimpleMath::Vector2(LIGHTBALL_POSITION));
	}

	// --------------------------------------- //
	// 机と接触中にEキーを押したら地図を表示
	// --------------------------------------- //
	if (HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[6].GetBoundingOrientedBox()) && kb.E)
	{
		m_mapFlag = true;
	}
	// 「地図」画像の描画
	if (m_mapFlag)
	{
		m_spriteBatch->Draw(m_mapSRV.Get(), SimpleMath::Vector2(MAP_POSITION));
	}
	// 「地図」画像の削除
	if (m_mapFlag && kb.Enter)
	{
		m_mapFlag = false;
	}

	// -------------------------------- //
	// 画像の削除
	// -------------------------------- //
	// 「バールをこじ開けた」画像の削除
	if (m_barOpenFlag && kbTracker->pressed.Enter)
	{
		m_barOpenFlag = false;
		m_keygetFlag = true;
		m_keyFlag = true;
	}
	// 「鍵獲得」画像の削除
	else if (m_keygetFlag && kbTracker->pressed.Enter)
	{
		m_keygetFlag = false;
	}
	// 「鍵で開けた」画像の削除
	if (m_keyOpenFlag && kbTracker->pressed.Enter)
	{
		m_keyOpenFlag = false;
		m_lightgetFlag = true;
		m_lightBallFlag = true;
	}
	// 「光の玉獲得」画像の削除
	else if (m_lightgetFlag && kbTracker->pressed.Enter)
	{
		m_lightgetFlag = false;
	}

	// --------------------------------------- //
	// 「ステージクリア」画像の描画
	// --------------------------------------- //
	if (HitCheckOBB(m_robotColl[1].GetBoundingOrientedBox(), m_modelColl[7].GetBoundingOrientedBox())
		&& m_lightBallFlag  && kb.E)
	{
		m_stageClearFlag = true;
	}
	if (m_stageClearFlag)
	{
		// 「光の玉」画像の削除
		m_lightBallFlag = false;
		m_spriteBatch->Draw(m_stageClearSRV.Get(), SimpleMath::Vector2(STAGECLEAR_POSITION));
	}

	m_spriteBatch->End();

	// 衝突判定の登録（デバッグ用）
	//CollisionRegist();
}

void GameStageScene::Finalize()
{
	m_headModel.reset();
	m_bodyModel.reset();
	m_legModel.reset();
	m_armRModel.reset();
	m_armLModel.reset();
	m_lightModel.reset();
	m_missileModel.reset();

	m_floorModel.reset();
	m_wallModel.reset();

	m_lanternModel.reset();
	m_deskModel.reset();
	m_treasureModel.reset();
	m_WECBoxModel.reset();
	m_woodBoxModel.reset();
	m_cabinetModel.reset();
	m_pottedModel.reset();
	m_bedModel.reset();
	m_lanternModel.reset();
	m_deskModel.reset();

	m_robotMoveSRV.Reset();
	m_upbodySRV.Reset();
	m_ArmLSRV.Reset();
	m_eCheckSRV.Reset();
	m_lightBallSRV.Reset();
	m_nothingSRV.Reset();
	m_lightgetSRV.Reset();
	m_keygetSRV.Reset();
	m_keySRV.Reset();
	m_lockSRV.Reset();
	m_eSetSRV.Reset();
	m_stageClearSRV.Reset();
	m_notgetSRV.Reset();
	m_guideSRV.Reset();
	m_itemFrameSRV.Reset();
	m_crowBarSRV.Reset();
	m_bargetSRV.Reset();
	m_stuckSRV.Reset();
	m_barOpenSRV.Reset();
	m_mapSRV.Reset();

	// 影の終了処理
	ResetShadow();
}

void GameStageScene::CreateDeviceDependentResources()
{
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();

	// エフェクトファクトリーの作成(モデル用)
	EffectFactory fx(device);
	fx.SetDirectory(L"Resources/Models");

	// 床のモデルの作成
	m_floorModel = Model::CreateFromCMO(device, L"Resources/Models/Floor.cmo", fx);
	// 壁
	m_wallModel = Model::CreateFromCMO(device, L"Resources/Models/Wall.cmo", fx);
	// 宝箱の作成
	m_treasureModel = Model::CreateFromCMO(device, L"Resources/Models/Treasure.cmo", fx);
	// WEC木箱の作成
	m_WECBoxModel = Model::CreateFromCMO(device, L"Resources/Models/WECBox.cmo", fx);
	// 木箱の作成
	m_woodBoxModel = Model::CreateFromCMO(device, L"Resources/Models/WoodBox.cmo", fx);
	// 戸棚の作成
	m_cabinetModel = Model::CreateFromCMO(device, L"Resources/Models/Cabinet.cmo", fx);
	// 鉢植えの作成
	m_pottedModel = Model::CreateFromCMO(device, L"Resources/Models/Potted.cmo", fx);
	// ベッドの作成
	m_bedModel = Model::CreateFromCMO(device, L"Resources/Models/Bed.cmo", fx);
	// ランタンの作成
	m_lanternModel = Model::CreateFromCMO(device, L"Resources/Models/Lantern.cmo", fx);
	// 机の作成
	m_deskModel = Model::CreateFromCMO(device, L"Resources/Models/Desk.cmo", fx);

	// スポットライトのモデルの作成
	m_spotLightModel = Model::CreateFromCMO(device, L"Resources/Models/SpotLight.cmo", fx);


	// ---------------------------------------------------------------------- //
	// ロボットのモデルの作成
	// ---------------------------------------------------------------------- //
	// エフェクトファクトリーの作成(ロボット用)
	std::unique_ptr<EffectFactory> fxrbt = std::make_unique<EffectFactory>(device);
	fxrbt->SetDirectory(L"Resources/Models");

	// ロボットの各パーツモデルの作成
	m_headModel = Model::CreateFromCMO(device, L"Resources/Models/Head.cmo", *fxrbt);
	m_bodyModel = Model::CreateFromCMO(device, L"Resources/Models/Body.cmo", *fxrbt);
	m_legModel = Model::CreateFromCMO(device, L"Resources/Models/Leg.cmo", *fxrbt);
	m_armRModel = Model::CreateFromCMO(device, L"Resources/Models/Arm_R.cmo", *fxrbt);
	m_armLModel = Model::CreateFromCMO(device, L"Resources/Models/Arm_L.cmo", *fxrbt);
	m_lightModel = Model::CreateFromCMO(device, L"Resources/Models/SpotLight.cmo", *fxrbt);
	m_missileModel = Model::CreateFromCMO(device, L"Resources/Models/Missile.cmo", *fxrbt);

	// 衝突判定の表示オブジェクトの作成
	//m_displayCollision = std::make_unique<Imase::DisplayCollision>(device, context);

	// 影の初期化関数
	InitializeShadow(device, context);

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

	// --------------------------------------------------------------------------- //
	// 各画像の作成
	// --------------------------------------------------------------------------- //
	// 「ロボットの移動」画像の作成
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/WASD.dds", nullptr, m_robotMoveSRV.ReleaseAndGetAddressOf())
	);
	// 「ロボットの上半身の回転」画像の作成
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/robot_RL.dds", nullptr, m_upbodySRV.ReleaseAndGetAddressOf())
	);
	// 「左腕(ライト)の回転」画像の作成
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/ArmL_UD.dds", nullptr, m_ArmLSRV.ReleaseAndGetAddressOf())
	);
	// 「E調べる」画像の作成
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/E_Check.dds", nullptr, m_eCheckSRV.ReleaseAndGetAddressOf())
	);
	// 「光の玉」画像の作成
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/LightBall.dds", nullptr, m_lightBallSRV.ReleaseAndGetAddressOf())
	);
	// 「何もないようだ」画像の作成
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/NotingGuide.dds", nullptr, m_nothingSRV.ReleaseAndGetAddressOf())
	);
	// 「光の玉獲得」画像の作成
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/LightGetGuide.dds", nullptr, m_lightgetSRV.ReleaseAndGetAddressOf())
	);
	// 「鍵獲得」画像の作成
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/KeyGetGuide.dds", nullptr, m_keygetSRV.ReleaseAndGetAddressOf())
	);
	// 「鍵がかかっている」画像の作成
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/LockedGuide.dds", nullptr, m_lockSRV.ReleaseAndGetAddressOf())
	);
	// 「鍵」画像の作成
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/key.dds", nullptr, m_keySRV.ReleaseAndGetAddressOf())
	);
	// 「Eはめる」画像の作成
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/E_Set.dds", nullptr, m_eSetSRV.ReleaseAndGetAddressOf())
	);
	// 「ステージクリア」画像の作成
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/STAGECLEAR.dds", nullptr, m_stageClearSRV.ReleaseAndGetAddressOf())
	);
	// 「何も起きない」画像の作成
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/NotingHappen.dds", nullptr, m_notgetSRV.ReleaseAndGetAddressOf())
	);
	// 「スタート案内」画像の作成
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/StartGuide.dds", nullptr, m_guideSRV.ReleaseAndGetAddressOf())
	);
	// 「アイテムフレーム」画像の作成
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/ItemFrame.dds", nullptr, m_itemFrameSRV.ReleaseAndGetAddressOf())
	);
	// 「バール」画像の作成
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/CrowBar.dds", nullptr, m_crowBarSRV.ReleaseAndGetAddressOf())
	);
	// 「バール」画像の作成
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/CrawBarGetGuide.dds", nullptr, m_bargetSRV.ReleaseAndGetAddressOf())
	);
	// 「画像引っ掛かっていて開かない」画像の作成
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/StuckGuide.dds", nullptr, m_stuckSRV.ReleaseAndGetAddressOf())
	);
	// 「バールをこじ開けた」画像の作成
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/BarOpenGuide.dds", nullptr, m_barOpenSRV.ReleaseAndGetAddressOf())
	);
	// 「鍵で開けた」画像の作成
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/KeyOpenGuide.dds", nullptr, m_keyOpenSRV.ReleaseAndGetAddressOf())
	);
	// 「鍵で開けた」画像の作成
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/MapGuide.dds", nullptr, m_mapSRV.ReleaseAndGetAddressOf())
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

void GameStageScene::CreateWindowSizeDependentResources()
{
	// 射影行列を作成
	RECT rect = GetUserResources()->GetDeviceResources()->GetOutputSize();
	m_proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 200.0f
	);
}

void GameStageScene::OnDeviceLost()
{
	Finalize();
}

//// 衝突判定の登録（デバッグ用）
//void GameStageScene::CollisionRegist()
//{
//	// --------------------------------------------------------------- //
//	// 衝突判定の登録
//	// --------------------------------------------------------------- //
//	// ロボット
//	m_displayCollision->AddBoundingOrientedBox(m_robotColl[0].GetBoundingOrientedBox()); // ロボット（体）
//	m_displayCollision->AddBoundingOrientedBox(m_robotColl[1].GetBoundingOrientedBox()); // ロボット（脚）
//	m_displayCollision->AddBoundingOrientedBox(m_robotColl[2].GetBoundingOrientedBox()); // ロボット（右腕）
//	m_displayCollision->AddBoundingOrientedBox(m_robotColl[3].GetBoundingOrientedBox()); // ロボット（左腕）
//	m_displayCollision->AddBoundingOrientedBox(m_robotColl[4].GetBoundingOrientedBox()); // ロボット（ミサイル）
//	// 各モデル
//	m_displayCollision->AddBoundingBox(m_modelColl[0].GetBoundingBox()); // 宝箱
//	m_displayCollision->AddBoundingBox(m_modelColl[1].GetBoundingBox()); // WEC木箱
//	m_displayCollision->AddBoundingBox(m_modelColl[2].GetBoundingBox()); // 木箱
//	m_displayCollision->AddBoundingBox(m_modelColl[3].GetBoundingBox()); // 戸棚
//	m_displayCollision->AddBoundingBox(m_modelColl[4].GetBoundingBox()); // 鉢植え
//	m_displayCollision->AddBoundingBox(m_modelColl[5].GetBoundingBox()); // ベッド
//	m_displayCollision->AddBoundingBox(m_modelColl[6].GetBoundingBox()); // 机
//	m_displayCollision->AddBoundingBox(m_modelColl[7].GetBoundingBox()); // ランタン
//	// 壁
//	m_displayCollision->AddBoundingOrientedBox(m_wallColl[0].GetBoundingOrientedBox());
//	m_displayCollision->AddBoundingOrientedBox(m_wallColl[1].GetBoundingOrientedBox());
//	m_displayCollision->AddBoundingOrientedBox(m_wallColl[2].GetBoundingOrientedBox());
//	m_displayCollision->AddBoundingOrientedBox(m_wallColl[3].GetBoundingOrientedBox());
//
//	//// 衝突判定の表示
//	//auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
//	//auto states = GetUserResources()->GetCommonStates();
//	//m_displayCollision->DrawCollision(context, states, m_view, m_proj);
//}

// OBB判定関数
bool GameStageScene::HitCheckOBB(
	const DirectX::BoundingOrientedBox& box1,
	const DirectX::BoundingOrientedBox& box2
)
{
	return box1.Intersects(box2);
}

// 影の初期化関数
void GameStageScene::InitializeShadow(ID3D11Device* device, ID3D11DeviceContext* context)
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
void GameStageScene::ResetShadow()
{
	m_basicEffect.reset();
	m_primitiveBatch.reset();
}

// 影の描画関数
void GameStageScene::DrawShadow(
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

	// 深度バッファの設定
	context->OMSetDepthStencilState(states->DepthRead(), 0);
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
