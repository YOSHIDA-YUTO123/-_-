//===================================================
//
// 3Dポリゴンのファクトリー [factory_mesh_quad.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "factory_mesh_quad.h"
#include "texture_id_component.hpp"
#include "texture_manager.h"
#include "manager.h"
#include "tag_component.hpp"
#include "vertex_buffer_component.hpp"
#include "transform_component.hpp"
#include "size_component.hpp"
#include "layer_component.hpp"
#include "mesh_layer.hpp"
#include "vertex_manager.h"
#include "renderer_component.hpp"
#include "shadow_component.hpp"
#include "timer_component.hpp"
#include "color_component.hpp"
#include "sheet_component.hpp"
#include "texture_mt_manager.h"
#include "mrt_target_component.hpp"
#include "texture_mt_component.hpp"
#include "ui_wall_component.hpp"

//===================================================
// 3Dポリゴンの生成
//===================================================
entt::entity FactoryMeshQuad::Quad(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& Size, const char* pTexturePath)
{
	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	int nTextureID = -1;

	if (pTextureManager != nullptr)
	{
		// テクスチャのIDの設定
		nTextureID = pTextureManager->Register(pTexturePath);
	}

	// entityの生成
	const entt::entity entity = registry.create();

	registry.emplace<Tag::MeshQuadTag>(entity);
	registry.emplace<Transform3DComponent>(entity, pos);
	registry.emplace<VertexBufferComponent>(entity);
	registry.emplace<Size3DComponent>(entity, Size);
	registry.emplace<LayerComponent>(entity);
	registry.emplace<TextureIDComponent>(entity, nTextureID);
	registry.emplace<RendererTag::MeshQuadTag>(entity);
	

	// 初期化処理
	if (FAILED(InitMeshQuad::Init(registry, entity)))
	{
		assert(false);
	}
	return entity;
}

//===================================================
// 影の生成処理
//===================================================
entt::entity FactoryMeshQuad::Shadow(entt::registry& registry, const entt::entity owner, const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const float fMaxHeight)
{
	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	int nTextureID = -1;

	if (pTextureManager != nullptr)
	{
		// テクスチャのIDの設定
		nTextureID = pTextureManager->Register("data/TEXTURE/effect000.jpg");
	}

	// entityの生成
	const entt::entity entity = registry.create();

	registry.emplace<Tag::MeshQuadTag>(entity);
	registry.emplace<RendererTag::MeshQuadTag>(entity);
	registry.emplace<ColorComponent>(entity);
	registry.emplace<ShadowComponent>(entity, owner, fMaxHeight, size.x);
	registry.emplace<Transform3DComponent>(entity, pos);
	registry.emplace<VertexBufferComponent>(entity);
	registry.emplace<Size3DComponent>(entity, D3DXVECTOR3(size.x, 0.0f, size.y));
	registry.emplace<LayerComponent>(entity);
	registry.emplace<TextureIDComponent>(entity, nTextureID);
	registry.emplace<RendererComponent>(entity, RendererComponent::TYPE_SUB_BLEND);
	

	// 初期化処理
	if (FAILED(InitMeshQuad::Init(registry, entity)))
	{
		assert(false);
	}
	return entity;
}

//===================================================
// タイマーの生成
//===================================================
entt::entity FactoryMeshQuad::Timer(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const int nTime, const char* pTexturePath)
{
	// entityの生成
	const entt::entity entity = registry.create();

	auto& timerComp = registry.emplace<TimerComponent>(entity, nTime);
	registry.emplace<MRTTargetComponent>(entity,MRTType::TYPE_TIMER);
	registry.emplace<Tag::TimerTag>(entity);
	

	int nCount = 0;

	// 横幅の割合を求める
	float fRateWidth = size.x / TimerComponent::MAX_DIGIT;

	// 秒数分回す
	for (auto& second : timerComp.aSecondID)
	{
		// 計算用の位置
		D3DXVECTOR3 posWk = pos;

		// 位置のオフセット
		float fOffsetX = fRateWidth * 2.0f * nCount;

		// 位置を計算
		posWk.x = pos.x - fOffsetX;

		// シートの生成
		second = Sheet(registry, posWk, { fRateWidth ,size.y,size.z }, pTexturePath, 9, 1, true);
		nCount++;
	}

	// 秒数分回す
	for (auto& minute : timerComp.aMinuteID)
	{
		// 計算用の位置
		D3DXVECTOR3 posWk = pos;

		// 位置のオフセット
		float fOffsetX = fRateWidth * 2.0f * nCount;

		// 位置を計算
		posWk.x = pos.x - fOffsetX;

		// シートの生成
		minute = Sheet(registry, posWk, { fRateWidth ,size.y,size.z }, pTexturePath, 9, 1, true);

		nCount++;
	}

	return entity;
}

//===================================================
// 分割3Dポリゴンの初期化処理
//===================================================
entt::entity FactoryMeshQuad::Sheet(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const char* pTexturePath, const int nSegmentU, const int nSegmentV, const bool bRendererMT)
{
	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	int nTextureID = -1;

	if (pTextureManager != nullptr)
	{
		// テクスチャのIDの設定
		nTextureID = pTextureManager->Register(pTexturePath);
	}

	// entityの生成
	const entt::entity entity = registry.create();

	registry.emplace<Tag::MeshQuadTag>(entity);
	

	if (!bRendererMT)
	{
		registry.emplace<RendererTag::MeshQuadTag>(entity);
		registry.emplace<LayerComponent>(entity);
	}

	registry.emplace<VertexBufferComponent>(entity);
	registry.emplace<SheetComponent>(entity, nSegmentU, nSegmentV);
	registry.emplace<Transform3DComponent>(entity, pos);
	registry.emplace<Size3DComponent>(entity, size);
	registry.emplace<TextureIDComponent>(entity, nTextureID);
	registry.emplace<ColorComponent>(entity);

	// 初期化に失敗したら
	if (FAILED(InitMeshQuad::Init(registry, entity, nSegmentU, nSegmentV)))
	{
		registry.destroy(entity);
		assert(false);
	}

	return entity;
}

//===================================================
// UIの壁の生成処理
//===================================================
entt::entity FactoryMeshQuad::UIWall(
	entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& size, 
	const char* pTexturePath, const char* pTexturePath2, 
	const D3DXVECTOR2& offPosUVLeft, const D3DXVECTOR2& offPosUVRight, const D3DXVECTOR3& rot)
{
	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	int nTextureID = -1;
	int nTextureID2 = -1;

	if (pTextureManager != nullptr)
	{
		// テクスチャのIDの設定
		nTextureID = pTextureManager->Register(pTexturePath);
		nTextureID2 = pTextureManager->Register(pTexturePath2);
	}

	// entityの生成
	const entt::entity entity = registry.create();

	registry.emplace<Transform3DComponent>(entity, pos, rot);
	registry.emplace<VertexBufferComponent>(entity);
	registry.emplace<Size3DComponent>(entity, size);
	registry.emplace<LayerComponent>(entity);
	registry.emplace<TextureMTComponent>(entity, nTextureID, nTextureID2);
	registry.emplace<RendererTag::MeshQuadTag>(entity);
	registry.emplace<UIWallComponent>(entity, D3DXVECTOR2(0.001f, 0.0f));

	// 初期化処理
	if (FAILED(InitMeshQuad::InitUIWall(registry, entity, offPosUVLeft, offPosUVRight)))
	{
		assert(false);
	}

	return entity;
}

//===================================================
// 3Dポリゴンの初期化処理
//===================================================
HRESULT InitMeshQuad::Init(entt::registry& registry, entt::entity entity, const int nSegmentU, const int nSegmentV)
{
	// 3Dポリゴンの生成処理
	if (FAILED(VertexManager::CreateQuadVtxBuffer(registry, entity)))
	{
		return E_FAIL;
	}

	// コンポーネントの取得
	auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);
	auto& sizeComp = registry.get<Size3DComponent>(entity);

	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファのロック
	vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 大きさの取得
	D3DXVECTOR3 Size = sizeComp.size;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-Size.x, Size.y, Size.z);
	pVtx[1].pos = D3DXVECTOR3(Size.x, Size.y, Size.z);
	pVtx[2].pos = D3DXVECTOR3(-Size.x, -Size.y, -Size.z);
	pVtx[3].pos = D3DXVECTOR3(Size.x, -Size.y, -Size.z);

	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = Const::WHITE;
	pVtx[1].col = Const::WHITE;
	pVtx[2].col = Const::WHITE;
	pVtx[3].col = Const::WHITE;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f / nSegmentU, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f / nSegmentV);
	pVtx[3].tex = D3DXVECTOR2(1.0f / nSegmentU, 1.0f / nSegmentV);

	// 頂点バッファのアンロック
	vertexBufferComp.pVtxBuffer->Unlock();

	return S_OK;
}

//===================================================
// マルチテクスチャの初期化処理
//===================================================
HRESULT InitMeshQuad::InitMT(entt::registry& registry, entt::entity entity, const int nSegmentU, const int nSegmentV)
{
	// コンポーネントの取得
	auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);

	// 3Dポリゴンの生成処理
	if (FAILED(VertexManager::CreateQuadVtxBufferMT(vertexBufferComp)))
	{
		return E_FAIL;
	}

	auto& sizeComp = registry.get<Size3DComponent>(entity);

	// 頂点情報のポインタ
	VERTEX_3D_MULT* pVtx = nullptr;

	// 頂点バッファのロック
	vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 大きさの取得
	D3DXVECTOR3 Size = sizeComp.size;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-Size.x, Size.y, -Size.z);
	pVtx[1].pos = D3DXVECTOR3(Size.x, Size.y, -Size.z);
	pVtx[2].pos = D3DXVECTOR3(-Size.x, -Size.y, Size.z);
	pVtx[3].pos = D3DXVECTOR3(Size.x, -Size.y, Size.z);

	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = Const::WHITE;
	pVtx[1].col = Const::WHITE;
	pVtx[2].col = Const::WHITE;
	pVtx[3].col = Const::WHITE;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f / nSegmentU, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f / nSegmentV);
	pVtx[3].tex = D3DXVECTOR2(1.0f / nSegmentU, 1.0f / nSegmentV);
	
	// テクスチャ座標の設定
	pVtx[0].texMT = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].texMT = D3DXVECTOR2(1.0f / nSegmentU, 0.0f);
	pVtx[2].texMT = D3DXVECTOR2(0.0f, 1.0f / nSegmentV);
	pVtx[3].texMT = D3DXVECTOR2(1.0f / nSegmentU, 1.0f / nSegmentV);

	// 頂点バッファのアンロック
	vertexBufferComp.pVtxBuffer->Unlock();

	return S_OK;
}

//===================================================
// UIの壁の生成処理
//===================================================
HRESULT InitMeshQuad::InitUIWall(entt::registry& registry, entt::entity entity, const D3DXVECTOR2& offPosUVLeft, const D3DXVECTOR2& offPosUVRight)
{
	// コンポーネントの取得
	auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);

	// 3Dポリゴンの生成処理
	if (FAILED(VertexManager::CreateQuadVtxBufferMT(vertexBufferComp)))
	{
		return E_FAIL;
	}

	auto& sizeComp = registry.get<Size3DComponent>(entity);

	// 頂点情報のポインタ
	VERTEX_3D_MULT* pVtx = nullptr;

	// 頂点バッファのロック
	vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 大きさの取得
	D3DXVECTOR3 Size = sizeComp.size;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-Size.x, Size.y, -Size.z);
	pVtx[1].pos = D3DXVECTOR3(Size.x, Size.y, -Size.z);
	pVtx[2].pos = D3DXVECTOR3(-Size.x, -Size.y, Size.z);
	pVtx[3].pos = D3DXVECTOR3(Size.x, -Size.y, Size.z);

	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = Const::WHITE;
	pVtx[1].col = Const::WHITE;
	pVtx[2].col = Const::WHITE;
	pVtx[3].col = Const::WHITE;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(offPosUVLeft.x, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(offPosUVRight.x, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(offPosUVLeft.x, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(offPosUVRight.x, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].texMT = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].texMT = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].texMT = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].texMT = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファのアンロック
	vertexBufferComp.pVtxBuffer->Unlock();

	return S_OK;
}
