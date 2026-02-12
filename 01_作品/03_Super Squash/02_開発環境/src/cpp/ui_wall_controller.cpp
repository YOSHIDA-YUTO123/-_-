//===================================================
//
// UIの壁の制御処理 [ui_wall_controller.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "ui_wall_controller.h"
#include "ui_wall_component.hpp"
#include "texture_manager.h"
#include "manager.h"
#include "entity_name_component.hpp"
#include "wall_constants.hpp"
#include "vertex_buffer_component.hpp"
#include "mesh_vtx_component.hpp"
#include "texture_mt_manager.h"

//===================================================
// タイトルの壁の設定処理
//===================================================
void UIWallController::SetTitleWall(entt::registry& registry)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = pManager->GetTextureManager();

	// 取得できないなら処理しない
	if (pTextureManager == nullptr) return;

	// 指定したコンポーネントをもつエンティティの取得
	auto view = registry.view<UIWallComponent>();

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& uiWallComp = registry.get<UIWallComponent>(entity);
		auto& entityNameComp = registry.get<EntityNameComponent>(entity);

		// 天井なら処理を飛ばす
		if (entityNameComp.name == WallConst::TOP_WALL)
		{
			continue;
		}
		
		// テクスチャのIDの取得
		int nTextureID = pTextureManager->Register("data/TEXTURE/title_UI.png");
		int nTextureMTID = pTextureManager->Register("data/TEXTURE/backgrond003.png");

		// テクスチャのIDの設定
		uiWallComp.pTexture = pTextureManager->GetAdress(nTextureID);
		uiWallComp.pTextureMT = pTextureManager->GetAdress(nTextureMTID);

		uiWallComp.bScrol = true;
		uiWallComp.bBackgrondScrol = true;
	}
}

//===================================================
// チュートリアルの壁の設定処理
//===================================================
void UIWallController::SetTutorialWall(entt::registry& registry)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = pManager->GetTextureManager();

	// 取得できないなら処理しない
	if (pTextureManager == nullptr) return;

	// 指定したコンポーネントをもつエンティティの取得
	auto view = registry.view<UIWallComponent>();

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& uiWallComp = registry.get<UIWallComponent>(entity);
		auto& entityNameComp = registry.get<EntityNameComponent>(entity);

		// 天井なら処理を飛ばす
		if (entityNameComp.name == WallConst::TOP_WALL)
		{
			continue;
		}

		// テクスチャのIDの取得
		int nTextureID = pTextureManager->Register("data/TEXTURE/tutorial_start.png");
		int nTextureMTID = pTextureManager->Register("data/TEXTURE/backgrond003.png");

		// テクスチャのIDの設定
		uiWallComp.pTexture = pTextureManager->GetAdress(nTextureID);
		uiWallComp.pTextureMT = pTextureManager->GetAdress(nTextureMTID);

		uiWallComp.bScrol = true;
		uiWallComp.bBackgrondScrol = true;
		uiWallComp.scrolSpeed.x = WallConst::TUTORIAL_SCROL_SPEED;
	}

	// UV座標の設定
	SetScrolUV(registry, 0.45f);
}

//===================================================
// 移動のタスクの設定
//===================================================
void UIWallController::SetTutorialTask(entt::registry& registry, const char* pTexturePath)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = pManager->GetTextureManager();

	// 取得できないなら処理しない
	if (pTextureManager == nullptr) return;

	// 指定したコンポーネントをもつエンティティの取得
	auto view = registry.view<UIWallComponent>();

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& uiWallComp = registry.get<UIWallComponent>(entity);
		auto& entityNameComp = registry.get<EntityNameComponent>(entity);

		// 天井なら処理を飛ばす
		if (entityNameComp.name == WallConst::TOP_WALL)
		{
			continue;
		}

		// テクスチャのIDの取得
		int nTextureID = -1;

		// 正面の壁だったら
		if (entityNameComp.name == WallConst::FRONT_WALL)
		{
			// テクスチャのIDの設定
			nTextureID = pTextureManager->Register(pTexturePath);
			uiWallComp.bBackgrondScrol = false;
		}
		else
		{
			// テクスチャのIDの設定
			nTextureID = pTextureManager->Register("data/TEXTURE/tutorial_skip.png");
		}

		int nTextureMTID = pTextureManager->Register("data/TEXTURE/backgrond003.png");

		// テクスチャの設定
		uiWallComp.pTexture = pTextureManager->GetAdress(nTextureID);
		uiWallComp.pTextureMT = pTextureManager->GetAdress(nTextureMTID);

		uiWallComp.bScrol = false;
		uiWallComp.scrolSpeed.x = WallConst::TUTORIAL_SCROL_SPEED;
	}

	// UV座標の設定
	SetNormalUV(registry);
}

//===================================================
// ゲームのルール説明の描画
//===================================================
void UIWallController::SetTutorialGameRule(entt::registry& registry)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = pManager->GetTextureManager();

	// 取得できないなら処理しない
	if (pTextureManager == nullptr) return;

	// 指定したコンポーネントをもつエンティティの取得
	auto view = registry.view<UIWallComponent>();

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& uiWallComp = registry.get<UIWallComponent>(entity);
		auto& entityNameComp = registry.get<EntityNameComponent>(entity);

		// 天井なら処理を飛ばす
		if (entityNameComp.name == WallConst::TOP_WALL)
		{
			continue;
		}

		// テクスチャのIDの取得
		int nTextureID = -1;
		int nTextureMTID = -1;

		// 正面の壁じゃないなら
		if (entityNameComp.name == WallConst::FRONT_WALL)
		{
			// テクスチャのIDの設定
			nTextureID = pTextureManager->Register("data/TEXTURE/tutorial_rule.png");
			nTextureMTID = pTextureManager->Register("data/TEXTURE/backgrond003.png");
		}
		else
		{
			// テクスチャのIDの設定
			nTextureID = pTextureManager->Register("data/TEXTURE/backgrond003.png");
		}

		// テクスチャの設定
		uiWallComp.pTexture = pTextureManager->GetAdress(nTextureID);
		uiWallComp.pTextureMT = pTextureManager->GetAdress(nTextureMTID);
	}
}

//===================================================
// ゲーム開始
//===================================================
void UIWallController::SetTutotialGameStart(entt::registry& registry)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = pManager->GetTextureManager();

	// 取得できないなら処理しない
	if (pTextureManager == nullptr) return;

	// 指定したコンポーネントをもつエンティティの取得
	auto view = registry.view<UIWallComponent>();

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& uiWallComp = registry.get<UIWallComponent>(entity);
		auto& entityNameComp = registry.get<EntityNameComponent>(entity);

		// 天井なら処理を飛ばす
		if (entityNameComp.name == WallConst::TOP_WALL)
		{
			continue;
		}

		// テクスチャのIDの設定
		int nTextureID = pTextureManager->Register("data/TEXTURE/tutorial_gamestart.png");
		int nTextureMTID = pTextureManager->Register("data/TEXTURE/background009.png");

		// テクスチャの設定
		uiWallComp.pTexture = pTextureManager->GetAdress(nTextureID);
		uiWallComp.pTextureMT = pTextureManager->GetAdress(nTextureMTID);

		uiWallComp.bScrol = true;
		uiWallComp.bBackgrondScrol = true;
		uiWallComp.scrolSpeed.x = WallConst::TUTORIAL_SCROL_SPEED;
	}

	// UV座標の設定
	SetScrolUV(registry, 0.7f);
}

//===================================================
// ゲーム開始のUI
//===================================================
void UIWallController::SetGameStart(entt::registry& registry)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = pManager->GetTextureManager();

	// マルチレンダーターゲットのテクスチャのマネージャーの取得
	CTextureMTManager* pTextureMTManager = pManager->GetTextureMTManager();

	// 取得できないなら処理しない
	if (pTextureManager == nullptr || pTextureMTManager == nullptr) return;

	// 指定したコンポーネントをもつエンティティの取得
	auto view = registry.view<UIWallComponent>();

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& uiWallComp = registry.get<UIWallComponent>(entity);
		auto& entityNameComp = registry.get<EntityNameComponent>(entity);

		// 天井なら処理を飛ばす
		if (entityNameComp.name == WallConst::TOP_WALL)
		{
			continue;
		}

		// テクスチャのIDの設定
		int nTextureID = pTextureManager->Register("data/TEXTURE/backgrond003.png");
		int nTextureMTID = pTextureManager->Register("data/TEXTURE/backgrond003.png");

		// 正面の壁だったら
		if (entityNameComp.name == WallConst::FRONT_WALL)
		{
			// テクスチャの設定
			uiWallComp.pTexture = pTextureMTManager->GetAdress(MRTType::TYPE_TIMER);
			uiWallComp.bScrol = false;
		}
		else
		{
			// テクスチャの設定
			uiWallComp.pTexture = pTextureManager->GetAdress(nTextureID);
			uiWallComp.bScrol = true;
		}

		uiWallComp.pTextureMT = pTextureManager->GetAdress(nTextureMTID);

		uiWallComp.bBackgrondScrol = true;
		uiWallComp.scrolSpeed.x = WallConst::TUTORIAL_SCROL_SPEED;
	}

	// UV座標の設定
	SetNormalUV(registry);
}

//===================================================
// リザルトの設定
//===================================================
void UIWallController::SetResultUI(entt::registry& registry)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = pManager->GetTextureManager();

	// マルチレンダーターゲットのマネージャークラスの取得
	CTextureMTManager* pTextureMTManager = pManager->GetTextureMTManager();

	// 取得できないなら処理しない
	if (pTextureManager == nullptr || pTextureMTManager == nullptr) return;

	// 指定したコンポーネントをもつエンティティの取得
	auto view = registry.view<UIWallComponent>();

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& uiWallComp = registry.get<UIWallComponent>(entity);
		auto& entityNameComp = registry.get<EntityNameComponent>(entity);

		// 天井なら処理を飛ばす
		if (entityNameComp.name == WallConst::TOP_WALL)
		{
			continue;
		}

		// 正面の壁
		if (entityNameComp.name == WallConst::FRONT_WALL)
		{
			uiWallComp.pTexture = pTextureMTManager->GetAdress(MRTType::TYPE_RESULT);
		}
		else
		{
			// テクスチャのIDの取得
			int nTextureID = pTextureManager->Register("data/TEXTURE/backgrond003.png");

			uiWallComp.pTexture = pTextureManager->GetAdress(nTextureID);
		}

		int nTextureMTID = pTextureManager->Register("data/TEXTURE/backgrond003.png");

		// テクスチャのIDの設定
		uiWallComp.pTextureMT = pTextureManager->GetAdress(nTextureMTID);

		uiWallComp.bScrol = false;
		uiWallComp.bBackgrondScrol = true;
		uiWallComp.scrolSpeed.x = WallConst::TUTORIAL_SCROL_SPEED;
	}

	// UV座標の設定
	SetNormalUV(registry);
}

//===================================================
// スクロール処理
//===================================================
void UIWallController::BeginResultScrol(entt::registry& registry)
{
	// 指定したコンポーネントをもつエンティティの取得
	auto view = registry.view<UIWallComponent>();

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& uiWallComp = registry.get<UIWallComponent>(entity);
		auto& entityNameComp = registry.get<EntityNameComponent>(entity);

		// 天井なら処理を飛ばす
		if (entityNameComp.name == WallConst::TOP_WALL)
		{
			continue;
		}

		uiWallComp.bScrol = true;
		uiWallComp.bBackgrondScrol = true;
	}

	// UV座標の設定
	SetScrolUV(registry, 0.45f);
}

//===================================================
// ランキングの設定
//===================================================
void UIWallController::SetRanking(entt::registry& registry)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = pManager->GetTextureManager();

	// マルチレンダーターゲットのテクスチャのマネージャーの取得
	CTextureMTManager* pTextureMTManager = pManager->GetTextureMTManager();

	// 取得できないなら処理しない
	if (pTextureManager == nullptr || pTextureMTManager == nullptr) return;

	// 指定したコンポーネントをもつエンティティの取得
	auto view = registry.view<UIWallComponent>();

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& uiWallComp = registry.get<UIWallComponent>(entity);
		auto& entityNameComp = registry.get<EntityNameComponent>(entity);

		// 天井なら処理を飛ばす
		if (entityNameComp.name == WallConst::TOP_WALL)
		{
			continue;
		}

		// 正面の壁だったら
		if (entityNameComp.name == WallConst::FRONT_WALL)
		{
			uiWallComp.pTexture = pTextureMTManager->GetAdress(MRTType::TYPE_RANKING);
		}
		else
		{
			// テクスチャのIDの取得
			int nTextureID = pTextureManager->Register("data/TEXTURE/tutorial_start.png");

			uiWallComp.pTexture = pTextureManager->GetAdress(nTextureID);
		}

		int nTextureMTID = pTextureManager->Register("data/TEXTURE/backgrond003.png");

		// テクスチャのIDの設定
		uiWallComp.pTextureMT = pTextureManager->GetAdress(nTextureMTID);
	}
}

//===================================================
// タイムリミットの設定
//===================================================
void UIWallController::SetTimeLimit60Second(entt::registry& registry)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = pManager->GetTextureManager();

	// マルチレンダーターゲットのテクスチャのマネージャーの取得
	CTextureMTManager* pTextureMTManager = pManager->GetTextureMTManager();

	// 取得できないなら処理しない
	if (pTextureManager == nullptr || pTextureMTManager == nullptr) return;

	// 指定したコンポーネントをもつエンティティの取得
	auto view = registry.view<UIWallComponent>();

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& uiWallComp = registry.get<UIWallComponent>(entity);
		auto& entityNameComp = registry.get<EntityNameComponent>(entity);

		// 天井なら処理を飛ばす
		if (entityNameComp.name == WallConst::TOP_WALL)
		{
			continue;
		}

		// テクスチャのIDの取得
		int nTextureID = pTextureManager->Register("data/TEXTURE/TimeLimit60.png");

		// テクスチャのIDの取得
		int nTextureMTID = pTextureManager->Register("data/TEXTURE/backgrond001.png");

		// テクスチャのポインタの設定
		uiWallComp.pTexture = pTextureManager->GetAdress(nTextureID);

		// テクスチャのポインタの設定
		uiWallComp.pTextureMT = pTextureManager->GetAdress(nTextureMTID);

		uiWallComp.bScrol = true;
	}

	// スクロール
	SetScrolUV(registry, 0.45f);
}

//===================================================
// 第２フェーズのUI
//===================================================
void UIWallController::SetSecondFaze(entt::registry& registry)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = pManager->GetTextureManager();

	// マルチレンダーターゲットのテクスチャのマネージャーの取得
	CTextureMTManager* pTextureMTManager = pManager->GetTextureMTManager();

	// 取得できないなら処理しない
	if (pTextureManager == nullptr || pTextureMTManager == nullptr) return;

	// 指定したコンポーネントをもつエンティティの取得
	auto view = registry.view<UIWallComponent>();

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& uiWallComp = registry.get<UIWallComponent>(entity);
		auto& entityNameComp = registry.get<EntityNameComponent>(entity);

		// 天井なら処理を飛ばす
		if (entityNameComp.name == WallConst::TOP_WALL)
		{
			continue;
		}

		// 正面の壁
		if (entityNameComp.name == WallConst::FRONT_WALL)
		{
			uiWallComp.bScrol = false;

			// テクスチャのポインタの設定
			uiWallComp.pTexture = pTextureMTManager->GetAdress(MRTType::TYPE_TIMER);

		}
		else
		{
			// テクスチャのIDの取得
			int nTextureID = pTextureManager->Register("data/TEXTURE/backgrond001.png");

			// テクスチャのポインタの設定
			uiWallComp.pTexture = pTextureManager->GetAdress(nTextureID);

			uiWallComp.bScrol = true;
		}


		// テクスチャのIDの取得
		int nTextureMTID = pTextureManager->Register("data/TEXTURE/backgrond001.png");


		// テクスチャのポインタの設定
		uiWallComp.pTextureMT = pTextureManager->GetAdress(nTextureMTID);

	}

	// スクロール
	SetNormalUV(registry);
}

//===================================================
// スクロールのUV座標の設定
//===================================================
void UIWallController::SetScrolUV(entt::registry& registry, const float fPosTexU)
{
	// 指定したコンポーネントをもつエンティティの取得
	auto view = registry.view<UIWallComponent>();

	// 頂点へのポインタ
	VERTEX_3D_MULT* pVtx = nullptr;

	// UVのオフセット
	D3DXVECTOR2 OffPosUVLeft = Const::VEC2_NULL;
	D3DXVECTOR2 OffPosUVRight = Const::VEC2_NULL;

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& meshVtxComp = registry.get<MeshVtxComponent>(entity);
		auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);
		auto& entityNameComp = registry.get<EntityNameComponent>(entity);

		// 天井だったら処理を飛ばす
		if (entityNameComp.name == WallConst::TOP_WALL)
		{
			continue;
		}

		// UVの設定
		if (entityNameComp.name == WallConst::LEFT_WALL)
		{
			OffPosUVLeft.x = 0.0f;
			OffPosUVRight.x = fPosTexU;
		}
		// UVの設定
		if (entityNameComp.name == WallConst::FRONT_WALL)
		{
			OffPosUVLeft.x = fPosTexU;
			OffPosUVRight.x = fPosTexU * 2.0f;
		}
		// UVの設定
		if (entityNameComp.name == WallConst::RIGHT_WALL)
		{
			OffPosUVLeft.x = fPosTexU * 2.0f;
			OffPosUVRight.x = fPosTexU * 3.0f;
		}
		// UVの設定
		if (entityNameComp.name == WallConst::BACK_WALL)
		{
			OffPosUVLeft.x = fPosTexU * 3.0f;
			OffPosUVRight.x = fPosTexU * 4.0f;
		}

		OffPosUVLeft.y = 0.0f;
		OffPosUVRight.y = 1.0f;

		// 頂点バッファのアンロック
		vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点のカウント
		int nCntVtx = 0;

		// テクスチャの座標の割合
		float fTexPosX = OffPosUVRight.x / meshVtxComp.nSegmentU;
		float fTexPosY = OffPosUVRight.y / meshVtxComp.nSegmentV;

		for (int nCntZ = 0; nCntZ <= meshVtxComp.nSegmentV; nCntZ++)
		{
			for (int nCntX = 0; nCntX <= meshVtxComp.nSegmentU; nCntX++)
			{
				// 割合を求める
				float fRateU = static_cast<float>(nCntX) / meshVtxComp.nSegmentU;
				float fRateV = static_cast<float>(nCntZ) / meshVtxComp.nSegmentV;

				float fPosU = OffPosUVLeft.x + (OffPosUVRight.x - OffPosUVLeft.x) * fRateU;
				float fPosV = OffPosUVLeft.y + (OffPosUVRight.y - OffPosUVLeft.y) * fRateV;

				// テクスチャ座標の設定
				pVtx[nCntVtx].tex = D3DXVECTOR2(fPosU, fPosV);

				// テクスチャ座標の設定
				pVtx[nCntVtx].texMT = D3DXVECTOR2((fTexPosX * nCntX), (fTexPosY * nCntZ));

				nCntVtx++;
			}
		}

		// 頂点バッファのアンロック
		vertexBufferComp.pVtxBuffer->Unlock();
	}
}

//===================================================
// 通常時の壁のUV座標
//===================================================
void UIWallController::SetNormalUV(entt::registry& registry)
{
	// 指定したコンポーネントをもつエンティティの取得
	auto view = registry.view<UIWallComponent>();

	// 頂点へのポインタ
	VERTEX_3D_MULT* pVtx = nullptr;

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& meshVtxComp = registry.get<MeshVtxComponent>(entity);
		auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);

		// 頂点のカウント
		int nCntVtx = 0;

		// テクスチャの座標の割合
		float fTexPosX = 1.0f / meshVtxComp.nSegmentU;
		float fTexPosY = 1.0f / meshVtxComp.nSegmentV;

		// 頂点バッファをロック
		vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntZ = 0; nCntZ <= meshVtxComp.nSegmentV; nCntZ++)
		{
			for (int nCntX = 0; nCntX <= meshVtxComp.nSegmentU; nCntX++)
			{
				// テクスチャ座標の設定
				pVtx[nCntVtx].tex = D3DXVECTOR2(fTexPosX * nCntX, fTexPosY * nCntZ);

				// テクスチャ座標の設定
				pVtx[nCntVtx].texMT = D3DXVECTOR2(fTexPosX * nCntX, fTexPosY * nCntZ);

				nCntVtx++;
			}
		}

		// 頂点バッファのアンロック
		vertexBufferComp.pVtxBuffer->Unlock();
	}
}