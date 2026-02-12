//===================================================
//
// UIの壁の更新処理 [ui_wall_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "ui_wall_system.h"
#include "ui_wall_component.hpp"
#include "vertex_buffer_component.hpp"
#include "mesh_vtx_component.hpp"
#include "entity_name_component.hpp"
#include "wall_constants.hpp"

//===================================================
// 更新処理
//===================================================
void UIWallSystem::Update(entt::registry& registry)
{
	// 指定したコンポーネントの取得
	auto view = registry.view<UIWallComponent>();

	VERTEX_3D_MULT* pVtx = nullptr;

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& UIWallComp = registry.get<UIWallComponent>(entity);
		auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);
		auto& entityNameComp = registry.get<EntityNameComponent>(entity);

		// 天井だったら処理を飛ばす
		if (entityNameComp.name == WallConst::TOP_WALL)
		{
			continue;
		}
#if 0
		// 頂点バッファのアンロック
		vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャのスクロール
		pVtx[0].tex -= UIWallComp.ScrolSpeed;
		pVtx[1].tex -= UIWallComp.ScrolSpeed;
		pVtx[2].tex -= UIWallComp.ScrolSpeed;
		pVtx[3].tex -= UIWallComp.ScrolSpeed;

		// 
		pVtx[0].texMT += UIWallComp.ScrolSpeed;
		pVtx[1].texMT += UIWallComp.ScrolSpeed;
		pVtx[2].texMT += UIWallComp.ScrolSpeed;
		pVtx[3].texMT += UIWallComp.ScrolSpeed;

		// 頂点バッファのロック
		vertexBufferComp.pVtxBuffer->Unlock();
#else
		// 頂点バッファのアンロック
		vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		auto& meshVtxComp = registry.get<MeshVtxComponent>(entity);

		for (int nCnt = 0; nCnt < meshVtxComp.nNumVertex; nCnt++)
		{
			// スクロールするなら
			if (UIWallComp.bScrol)
			{
				// テクスチャのスクロール
				pVtx[nCnt].tex -= UIWallComp.scrolSpeed;
			}
			// 背景スクロールするなら
			if (UIWallComp.bBackgrondScrol)
			{
				pVtx[nCnt].texMT += UIWallComp.scrolSpeed;
			}
		}

		// 頂点バッファのロック
		vertexBufferComp.pVtxBuffer->Unlock();

#endif // 0
	}
}